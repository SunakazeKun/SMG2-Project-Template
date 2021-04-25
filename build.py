import glob
import subprocess
import os.path
from os import path
import sys
from pathlib import Path

def err(str):
    print(f"Error: {str}")
    sys.exit(1)

region = sys.argv[1]
target = ""
if len(sys.argv) > 2:
    target = sys.argv[2] + "/"

if not os.path.exists("build"):
    os.mkdir("build")

if not os.path.exists("CodeWarrior/mwcceppc.exe"):
    err("CodeWarrior compiler not found.")

if not os.path.exists("Kamek/Kamek.exe"):
    err("Kamek not found.")

cpp_files = glob.glob(f"source/{target}*.cpp")

if len(cpp_files) <= 0:
    err("No C++ files to compile!") 

# CodeWarrior compiler options
options = " ".join([
    "-i .",
    "-I-",
    "-i include",
    "-nodefaults",
    "-proc gekko",  # set gekko processor core
    "-Cpp_exceptions off",
    "-enum int",
    "-O4,s",  # code size optimization level 4
    "-func_align 4",  # save space by aligning functions to 4 bytes instead of 16 bytes
    "-str pool",  # strings are collected in one pool per object to save space
    "-fp hard",
    "-sdata 0",
    "-sdata2 0",
    f"-D{region}",
    "-DGEKKO",
    "-DMTX_USE_PS",  # use paired singles
    "-MMD",
    "-rtti off",
    "-c",
    "-o"
])
cmd = f"CodeWarrior\mwcceppc.exe {options}"

for cpp_file in cpp_files:
    name = Path(cpp_file).stem
    print(f"Compiling {name}.cpp...")
    cmdNew = f"{cmd} build/{target}{name}.o source/{target}{name}.cpp"
    if subprocess.call(cmdNew, shell=True) != 0:
        err("Compiler error.")

print("Linking...")

o_files = glob.glob(f"build/{target}*.o")
o_str = ' '.join([str(e) for e in o_files])

kamek_cmd = f"Kamek\Kamek.exe {o_str} -externals=symbols/{region}.txt -output-kamek=CustomCode.bin"
if subprocess.call(kamek_cmd, shell=True) != 0:
    err("Linking failed.")

print("Done!")