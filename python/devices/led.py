#!/usr/bin/env python3
# coding: utf-8

import sys
import vdev
import struct
import time


LED_CMD_INIT   = 0
LED_CMD_ON     = 1
LED_CMD_OFF    = 2
LED_CMD_TOGGLE = 3


class Led(vdev.Device):

    def received(self, data):
        cmd = struct.unpack('B', data)[0]
        if cmd == LED_CMD_INIT:
            time.sleep(1)
            self.send(b'\x01')

    def run(self):
        print('run')
