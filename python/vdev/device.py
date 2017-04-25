#!/usr/bin/env python3
# coding: utf-8

import struct


class Device(object):

    __slots__ = ('model', 'dev_id')

    def received(self, dev_id, data):
        """You may rewrite this method
        """
        pass

    def set_send_options(self, recv_q, send_q, model):
        self.__recv_q = recv_q
        self.__send_q = send_q
        self.__model = model

    def send(self, data, dev_id):
        """Send data to client

        Parameters:
            data:   Received data
            dev_id: Device id
        """
        # check data validate
        if type(data) is not bytes:
            raise ValueError('data type must be bytes')

        # send data
        try:
            self.__send_q.put({
                'model': self.__model,
                'id': dev_id,
                'data': data
            })
        except NameError:
            raise RuntimeError("Please register device and run the app at first")

    def send_status(self, status, dev_id):
        """Send status code to client, refer to vdev_status_t
        """
        data = struct.pack('I', status)
        self.send(data, dev_id)

    def recv_loop(self):
        """listen receive queue.
        """
        while True:
            obj = self.__recv_q.get()
            dev_id = obj['dev_id']

            ret = self.received(**obj)
            # return data + status
            if type(ret) == tuple and len(ret) == 2 and type(ret[0]) is bytes and type(ret[1]) is int:
                self.send(*ret, dev_id)
            # return status
            elif type(ret) is int:
                self.send_status(ret, dev_id)
            elif type(ret) is bytes:
                self.send(ret, dev_id)
            else:
                raise RuntimeError('function "received()" return an error format')
