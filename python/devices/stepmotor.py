#!/usr/bin/env python3
# coding: utf-8

import sys
import vdev
import struct
import time


class Stepmotor(vdev.Device):
    STEPMOTOR_CMD_INIT   = 0
    STEPMOTOR_CMD_RUN    = 1

    def __init__(self, model, dev_id):
        self.count = 0
        super().__init__(model, dev_id)

    def received(self, data):
        cmd = struct.unpack('B', data)[0]
        if cmd == self.__class__.STEPMOTOR_CMD_INIT:
            self.count = 0
        elif cmd == self.__class__.STEPMOTOR_CMD_RUN:
            self.count += 1

        print('stepmotor %d: %.2f°' % (self.dev_id, self.count % 400 / 400 * 360))

        self.send_status(vdev.config.status.VDEV_STATUS_SUCCESS)

    def run(self):
        pass
