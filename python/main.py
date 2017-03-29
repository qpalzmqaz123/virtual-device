#!/usr/bin/env python3
# coding: utf-8

from vdev import Vdev, Device
import devices

if __name__ == '__main__':
    v = Vdev()

    v.register_device(devices.Led, 3, 0)

    v.start()
