#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import subprocess


if __name__ == '__main__':
    p1 = subprocess.Popen('./out.posix/simulator')
    p2 = subprocess.Popen('./python/main.py')
    p1.wait()
    p2.wait()
