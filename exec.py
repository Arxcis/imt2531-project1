#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
import subprocess
import os
import platform
import sys


def run(build_dir):
    cmd = [build_dir + "/main"]
    print("-- RUNNING GAME")
    subprocess.call(cmd)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-r", "--run", help="run only", action="store_true")
    argv = parser.parse_args()

    build_dir = "./bin"
    source_dir = "./src"

    if argv.run:
        run(build_dir)
    else:
        run(build_dir)
