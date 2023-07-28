import glob
import os
import shutil
import subprocess
import sys
from pathlib import Path
from elftools.elf.elffile import ELFFile


def err(message: str):
    print(f"Error: {message}")
    sys.exit(1)


def dep(path, name):
    path = Path(path)
    if not os.path.exists(path):
        err(f"{name} not found!")
    return path


def remove_d_files():
    for f in glob.glob("*.d"):
        os.remove(f)


# ----------------------------------------------------------------------------------------------------------------------
# Define paths and check dependencies
# ----------------------------------------------------------------------------------------------------------------------
REGIONS = ["PAL", "USA", "JPN", "TWN", "KOR"]

MWCCEPPC = dep("deps/CodeWarrior/mwcceppc.exe", "CodeWarrior compiler")
MWASMEPPC = dep("deps/CodeWarrior/mwasmeppc.exe", "CodeWarrior assembler")
KAMEK = dep("deps/Kamek/Kamek.exe", "Kamek linker")
SYATI_INCLUDE = dep("deps/Syati/include", "Syati include folder")
SYATI_SYMBOLS = dep("deps/Syati/symbols", "Syati symbols folder")


# ----------------------------------------------------------------------------------------------------------------------
# Define includes, compiler/assembler flags and fetch tasks
# ----------------------------------------------------------------------------------------------------------------------
include_paths = [".", "include", SYATI_INCLUDE]
includes = " -I- ".join([f"-i {i}" for i in include_paths])

compiler_flags = [
    "-c",
    "-Cpp_exceptions off",
    "-nodefaults",
    "-proc gekko",
    "-fp hard",
    "-lang=c++",
    "-O4,s",
    "-inline on",
    "-rtti off",
    "-sdata 0",
    "-sdata2 0",
    "-align powerpc",
    "-func_align 4",
    "-str pool",
    "-enum int",
    "-DGEKKO",
    "-DMTX_USE_PS",
]

assembler_flags = [
    "-c",
    "-proc gekko",
]

compile_cmd = f'{MWCCEPPC} {" ".join(compiler_flags)} {includes}'
assemble_cmd = f'{MWASMEPPC} {" ".join(assembler_flags)} {includes}'


compile_tasks = list()
assemble_tasks = list()

for root, _, files in os.walk("source"):
    for file in filter(lambda f: f.endswith(".cpp") or f.endswith(".s"), files):
        source_path = os.path.join(root, file)

        if file.endswith(".s"):
            build_path = source_path.replace("source", "build", 1).replace(".s", ".o")
            assemble_tasks.append((source_path, build_path))
        else:
            build_path = source_path.replace("source", "build", 1).replace(".cpp", ".o")
            compile_tasks.append((source_path, build_path))


# ----------------------------------------------------------------------------------------------------------------------
# Compiling, assembling and linking step
# ----------------------------------------------------------------------------------------------------------------------
def build(region: str):
    if os.path.exists("build"):
        shutil.rmtree("build", ignore_errors=True)

    print(f"Building target {region}")

    if len(compile_tasks) == 0:
        print("No C++ files to compile, skipping compiling...")
    else:
        for source_path, build_path in compile_tasks:
            os.makedirs(os.path.dirname(build_path), exist_ok=True)
            print(f"Compiling {source_path}...")

            if subprocess.call(f"{compile_cmd} -D{region} {source_path} -o {build_path}", shell=True) != 0:
                err("Compiler error.")
                remove_d_files()

    if len(assemble_tasks) == 0:
        print("No assembly files to compile, skipping assembling...")
    else:
        for source_path, build_path in assemble_tasks:
            os.makedirs(os.path.dirname(build_path), exist_ok=True)
            print(f"Assembling {source_path}...")

            if subprocess.call(f"{assemble_cmd} -D{region} {source_path} -o {build_path}", shell=True) != 0:
                err("Assembler error.")
                remove_d_files()

    print("Linking...")

    object_tasks = compile_tasks + assemble_tasks
    object_files = " ".join(task[1] for task in object_tasks)
    kamek_cmd = f"{KAMEK} {object_files} -externals={SYATI_SYMBOLS}/{region}.txt -output-kamek=CustomCode_{region}.bin"

    if subprocess.call(kamek_cmd, shell=True) != 0:
        err("Linking failed.")

    remove_d_files()

    make_map(region, [task[1] for task in object_tasks])

    print("Done!")


def make_map(region: str, object_paths: list[str]):
    section_names = [".init", ".fini", ".text", ".ctors", ".dtors", ".rodata", ".data", ".bss"]
    symbols_by_sections = {name: [] for name in section_names}
    section_sizes = {name: 0 for name in section_names}

    for object_path in object_paths:
        with open(object_path, "rb") as f:
            elf = ELFFile(f)

            for section_name in section_names:
                section = elf.get_section_by_name(section_name)
                symtab = elf.get_section_by_name(".symtab")

                if section is None:
                    continue

                local_section_base = section_sizes[section_name]
                total_section_size = (local_section_base + section.data_size + 3) & ~3
                section_sizes[section_name] = total_section_size

                for symbol in symtab.iter_symbols():
                    name = symbol.name
                    st_name = symbol.entry["st_name"]
                    st_shndx = symbol.entry["st_shndx"]
                    st_value = symbol.entry["st_value"]
                    st_size = symbol.entry["st_size"]

                    if st_name == 0 or st_size == 0 or st_shndx == "SHN_UNDEF":
                        continue
                    if not elf.get_section(st_shndx).name == section_name:
                        continue

                    offset = local_section_base + st_value
                    truncated_o_path = object_path.replace("\\", "/").removeprefix("build/")
                    symbols_by_sections[section_name].append((offset, st_size, name, truncated_o_path))

    with open(f"CustomCode_{region}.map", "w") as f:
        total_size = 0
        for section_name in section_names:
            section_size = section_sizes[section_name]

            if section_name == ".ctors":
                f.write(f"{section_name} section layout\n")
                f.write(f"  {total_size:08X}\t{section_size:06X}\t__ctor_loc\n")
                total_size += section_size
                f.write(f"  {total_size:08X}\t{0:06X}\t__ctor_end\n")
            else:
                if section_size == 0:
                    continue

                f.write(f"{section_name} section layout\n")

                for (offset, size, name, truncated_o_path) in sorted(symbols_by_sections[section_name], key=lambda s: s[0]):
                    absolute_offset = total_size + offset
                    f.write(f"  {absolute_offset:08X}\t{size:06X}\t{name}\t{truncated_o_path}\n")

                total_size += section_size


# ----------------------------------------------------------------------------------------------------------------------
# Entry point
# ----------------------------------------------------------------------------------------------------------------------
if len(sys.argv) < 2:
    print("Did not specify a target region, building all targets!")

    for region in REGIONS:
        build(region)
else:
    region = sys.argv[1]

    if region not in REGIONS:
        err(f"Invalid build target found: {region}")

    build(region)
