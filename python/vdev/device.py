#!/usr/bin/env python3
# coding: utf-8


class Device(object):
    """Vritual device class.
    """

    def recv_cb(self, data):
        pass

    def send(self, data):
        # TODO: set type
        try:
            self.__q.put(data)
        except NameError:
            raise RuntimeError("Please register device and run the app at first")

    def recv_loop(self, q, device_type, device_id):
        self.__q = q
        self.__type = device_type
        self.__id = device_id
        while True:
            data = self._q.get()
            self.recv_cb(data)
