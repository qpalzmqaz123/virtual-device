#!/usr/bin/env python3
# coding: utf-8


class Device(object):
    """Vritual device class.
    """

    def received(self, data):
        """You can rewrite this method
        """
        pass

    def run(self):
        """You can rewrite this method
        """
        pass

    def send(self, data):
        """Send data to client
        """
        # TODO: set type
        try:
            self.__q.put(data)
        except NameError:
            raise RuntimeError("Please register device and run the app at first")

