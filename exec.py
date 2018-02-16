#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
from subprocess import call
import os
import platform
import sys


def build():
    cmd = ["make"]
    call(cmd)

def run(build_dir):
    cmd = [build_dir + "/main"]
    print("-- RUNNING GAME")
    
    while call(cmd):
        pass


def compile_spritesheet(x,y,w,h,c,r, source, target):
    return


def install_libraries(ssh):

    if not os.path.isdir("./stb"):
        call(["mkdir", "./stb"])
        call(["curl", "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h", "-o", "./stb/stb_image.h"])
    else:
        print("-- stb_image.h already installed")

    if os.path.isdir("./glm") and os.path.isdir("./glfw") and os.path.isdir("./glew"):
        print("-- ./glew, ./glfw, ./glm already installed")
        return

    if ssh:
        call(["git", "clone", "git@prod3.imt.hig.no:imt2531/imt2531_lectures.git"])
    else: 
        call(["git", "clone", "http://prod3.imt.hig.no/imt2531/imt2531_lectures.git"])


    if not os.path.isdir("./glew"): 
        call(["mv", "./imt2531_lectures/lab03/glew", "."]) 
    else: 
        print("-- ./glew already installed")

    if not os.path.isdir("./glfw"): 
        call(["mv", "./imt2531_lectures/lab03/glfw", "."])
    else: 
        print("-- ./glfw already installed")

    if not os.path.isdir("./glm"):  
        call(["mv", "./imt2531_lectures/lab03/glm", "."])
    else:
        print("-- ./glm already installed")


    call(["rm", "-rf", "./imt2531_lectures/"])


if __name__ == "__main__":
    """
    compile_spritesheet(134, 234, 234, 234, 234,234, "pacman.png", "pacman.ost")
    compile_spritesheet(134, 234, 234, 234, 234,234, "pacman.png", "pacman.ost")
    compile_spritesheet(134, 234, 234, 234, 234,234, "pacman.png", "numbers.ost")
    """
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-r", "--run", help="run only", action="store_true")
    parser.add_argument("-i", "--install", help="install required libraries", action="store_true")
    parser.add_argument("--ssh", help="use ssh when git cloning", action="store_true")
    argv = parser.parse_args()

    build_dir = "./bin"
    source_dir = "./src"

    if argv.run:
        run(build_dir)
    elif argv.install:
        install_libraries(argv.ssh)
    else:
        build()
        run(build_dir)
