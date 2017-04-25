#!/usr/bin/env python3
# coding: utf-8

import sys
import vdev
import struct
import time


class Stepmotor(vdev.Device):

    def __init__(self):
        self.count = 0
        self.cmd = vdev.Enum2Py(
                vdev.config.vdev_root_dir + '/posix/stepmotor/posix_vdev_stepmotor.c',
                'stepmotor_cmd_t')

    def received(self, dev_id, data):
        cmd = struct.unpack('B', data)[0]

        if cmd == self.cmd.STEPMOTOR_CMD_INIT:
            self.count = 0
        elif cmd == self.cmd.STEPMOTOR_CMD_RUN:
            self.count += 1

        print('stepmotor %d: %.2f°' % (dev_id, self.count % 400 / 400 * 360))

        return vdev.config.status.VDEV_STATUS_SUCCESS

    def run(self):
        pass
