#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
import subprocess
import os
import platform
import sys


def build():
    cmd = ["make"]
    subprocess.call(cmd)

def run(build_dir):
    cmd = [build_dir + "/main"]
    print("-- RUNNING GAME")
    subprocess.call(cmd)

def compile_spritesheet(x,y,w,h,c,r, source, target):
    return


if __name__ == "__main__":
    """
    compile_spritesheet(134, 234, 234, 234, 234,234, "pacman.png", "pacman.ost")
    compile_spritesheet(134, 234, 234, 234, 234,234, "pacman.png", "pacman.ost")
    compile_spritesheet(134, 234, 234, 234, 234,234, "pacman.png", "numbers.ost")
    """
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-r", "--run", help="run only", action="store_true")
    parser.add_argument("-cs", "--compile-sprites", help="run only", action="store_true")
    argv = parser.parse_args()

    build_dir = "./bin"
    source_dir = "./src"

    if argv.run:
        run(build_dir)
    else:
        build()
        run(build_dir)
