#!/usr/bin/env python3
# coding: utf-8


class Device(object):
    """Vritual device class.
    """
    def __init__(self, model, dev_id):
        self.model = model
        self.dev_id = dev_id

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
        self.__model = model
        self.__dev_id = dev_id

    def send(self, data):
        """Send data to client
        """
        # check data validate
        if type(data) is not bytes:
            raise ValueError('data type must be bytes')

        # send data
        try:
            self.__q.put({
                'model': self.__model,
                'id': self.__dev_id,
                'data': data
            })
        except NameError:
            raise RuntimeError("Please register device and run the app at first")

