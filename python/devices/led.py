#!/usr/bin/env python3
# coding: utf-8

import sys
import vdev


class Led(vdev.Device):

    def received(self, data):
        print('recv')

    def run(self):
        print('run')
