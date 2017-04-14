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
    led_cmd_init = 0
    led_cmd_on = 1
    led_cmd_off = 2
    led_cmd_toggle = 3

    def __init__(self, model, dev_id):
        super().__init__(model, dev_id)

    def received(self, data):
        cmd = struct.unpack('B', data)[0]
        if cmd == self.__class__.led_cmd_init:
            # initial UI
            self.send(b'\x01')

    def run(self):
        print('run')
