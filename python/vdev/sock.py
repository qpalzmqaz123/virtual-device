#!/usr/bin/env python3
# coding: utf-8

import os
import socket
from . import config


class Sock(object):

    def __init__(self):
        self.s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        if os.path.exists(config.socket_file):
            os.unlink(config.socket_file)
        self.s.bind(config.socket_file)
        self.s.listen(1)

        print('Waiting the client...')
        self.conn = self.s.accept()[0]
        print('Connection established')

    def send(self, data):
        if type(data is not bytes):
            raise ValueError('data type must be bytes')
        self.conn.send(data)

    def recv(self):
        print(self.conn.recv(1024))
