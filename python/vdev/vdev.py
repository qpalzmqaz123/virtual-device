#!/usr/bin/env python3
# coding: utf-8

from multiprocessing import Process, Queue
from .sock import Sock

class Vdev(object):

    def __init__(self):
        # (device_type + device_id) => {'queue': xxx, 'class': xxx}
        self.table = {}
        self.s = Sock()

    def register_device(self, device_class, device_type, device_id=0):
        """Register virtual device.

        Parameters:
            device_class: device class name
            device_type: refer to config.py
            device_id: available when multi-device exist
        """
        self.table[device_type + device_id] = {
            'class': device_class,
            'queue': Queue()
        }

    def run(self):
        """Loop the socket forever, then distribute the data to each 
        callback via model and device id 
        """
        while True:
            self.s.recv()
        pass
