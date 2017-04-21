#!/usr/bin/env python3
# coding: utf-8

import struct


class Device(object):

    __slots__ = ('model', 'dev_id')

    def received(self, data):
        """You can rewrite this method
        """
        pass

    def run(self):
        """You can rewrite this method
        """
        pass

    def set_send_options(self, queue, model, dev_id):
        self.__q = queue
        self.model = model
        self.dev_id = dev_id

    def send(self, data):
        """Send data to client
        """
        # check data validate
        if type(data) is not bytes:
            raise ValueError('data type must be bytes')

        # send data
        try:
            self.__q.put({
                'model': self.model,
                'id': self.dev_id,
                'data': data
            })
        except NameError:
            raise RuntimeError("Please register device and run the app at first")

    def send_status(self, status):
        """Send status code to client, refer to vdev_status_t
        """
        data = struct.pack('I', status)
        self.send(data)

