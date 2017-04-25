#!/usr/bin/env python3
# coding: utf-8

from vdev import Vdev, Device, Enum2Py, config
import devices


if __name__ == '__main__':

    model = Enum2Py(config.vdev_root_dir + '/inc/vdev_model.h', 'vdev_model_t')

    v = Vdev()

    v.register_device(devices.Led, model.VDEV_MODEL_LED)
    v.register_device(devices.Stepmotor, model.VDEV_MODEL_STEPMOTOR)

    v.start()
