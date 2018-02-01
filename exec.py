#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
import subprocess
import os
import platform
import sys

def ensure_build_directory_exist(build_dir):

    if not os.path.isdir(build_dir):
        print("-- Creating build directory @ "+ build_dir)
        cmd = ["mkdir", build_dir]
        subprocess.call(cmd)


def build(build_dir, source_dir):

    cmd = []

    COMPILER      = ["g++"]
    CPP_VERSION   = ["-std=c++14"]
    DEBUG_FLAGS   = ["-g"]
    WARNING_FLAGS = ["-Wall"]
    INCLUDE_DIRS  = ["-I/usr/local/include"]
    LIBRARY_DIR   = ["-L/usr/local/lib"]

    UBUNTU_LINKER_FLAGS=["-lopengl", "-lglfw", "-lglew"]
    MACOS_LINKER_FLAGS=["-framework","OpenGL","-lglfw","-lglew"]

    OUTPUT_FILE  = ["-o", build_dir + "/main"]
    SOURCE_FILES = [source_dir + "/main.cpp"]

    cmd.extend(COMPILER)
    cmd.extend(CPP_VERSION)
    cmd.extend(DEBUG_FLAGS)
    cmd.extend(WARNING_FLAGS)
    cmd.extend(INCLUDE_DIRS)
    cmd.extend(LIBRARY_DIR)

    print("-- Building for " + platform.system());
    # LINUX
    if platform.system() == "Linux":
        cmd.extend(UBUNTU_LINKER_FLAGS)

    # MACOS
    elif platform.system() == "Darwin":
        cmd.extend(MACOS_LINKER_FLAGS)
    
    # WINDOWS AND FRIENDS
    else:
        print("-- "+ platform.system() + " not supported")
        sys.exit()

    cmd.extend(OUTPUT_FILE)
    cmd.extend(SOURCE_FILES)

    print("-- BUILD STARTING")
    subprocess.call(cmd)
    print("-- BUILD COMPLETE")

def run(build_dir):
    cmd = [build_dir + "/main"]

    print("-- RUNNING GAME")
    subprocess.call(cmd)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--build", help="build only", action="store_true")
    parser.add_argument("-r", "--run", help="run only", action="store_true")
    argv = parser.parse_args()

    build_dir = "./build"
    source_dir = "./src"

    ensure_build_directory_exist(build_dir)

    if argv.build:
        build(build_dir, source_dir)
    elif argv.run:
        run(build_dir)
    else:
        build(build_dir, source_dir)
        run(build_dir)
