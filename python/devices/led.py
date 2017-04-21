#!/usr/bin/env python3
# coding: utf-8

import sys
import vdev
import struct
import time
from PyQt5.QtWidgets import *
from threading import Event


class LedUI(QWidget):
    __instance = None

    def __new__(cls, *args, **kwargs):
        if not cls.__instance:
            QWidget.__new__(cls, *args, **kwargs)
        return cls.__instance

    def __init__(self):
        pass

class Led(vdev.Device):

    def __init__(self):
        self.state = False
        self.cmd = vdev.Enum2Py(
                vdev.config.vdev_root_dir + '/posix/led/posix_vdev_led.c',
                'led_cmd_t')

    def received(self, data):
        cmd = struct.unpack('B', data)[0]

        if cmd == self.cmd.LED_CMD_INIT:
            self.state = False
        elif cmd == self.cmd.LED_CMD_ON:
            self.state = True
        elif cmd == self.cmd.LED_CMD_OFF:
            self.state = False
        elif cmd == self.cmd.LED_CMD_TOGGLE:
            self.state = not self.state

        print('led%d: %s' % (self.dev_id, 'ON' if self.state else 'OFF'))

        self.send_status(vdev.config.status.VDEV_STATUS_SUCCESS)

    def run(self):
        pass
