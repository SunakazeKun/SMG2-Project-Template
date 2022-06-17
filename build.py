import glob
import os
import shutil
import subprocess
import sys
from pathlib import Path


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
REGIONS = ["PAL", "USA", "JAP", "TWN", "KOR"]

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

    object_files = " ".join(task[1] for task in compile_tasks + assemble_tasks)
    kamek_cmd = f"{KAMEK} {object_files} -externals={SYATI_SYMBOLS}/{region}.txt -output-kamek=CustomCode_{region}.bin"

    if subprocess.call(kamek_cmd, shell=True) != 0:
        err("Linking failed.")

    remove_d_files()
    print("Done!")


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
