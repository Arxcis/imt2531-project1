#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
import subprocess
import os
import platform
import sys

def create_build_directory_if_not_exist(build_dir):

    if not os.path.isdir(build_dir):
        print("-- Creating build directory @ "+ build_dir)
        cmd = ["mkdir", build_dir]
        subprocess.call(cmd)


def build(build_dir, source_dir):

    cmd = []
    cmd.extend(["g++", "-std=c++14", "-g", "-Wall",
                "-I/usr/local/include",
                "-L/usr/local/lib"])
    
    _platform = platform.system()

    print("-- Trying to build for " + _platform);

    #
    # LINUX
    #
    if _platform == "Linux":
        cmd.extend(["-lopengl", "-lglfw3", "-lglew"])

    #
    # MACOS
    #
    elif _platform == "Darwin":
        cmd.extend(["-framework","OpenGL","-lglfw","-lglew"])
    
    #
    # WINDOWS AND FRIENDS
    #
    else:
        print("-- "+ _platform + " not supported")
        sys.exit()


    cmd.extend(["-o", build_dir+"/main",
                 source_dir+"/main.cpp"])

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

    create_build_directory_if_not_exist(build_dir)

    if argv.build:
        build(build_dir, source_dir)
    elif argv.run:
        run(build_dir)
    else:
        build(build_dir, source_dir)
        run(build_dir)
