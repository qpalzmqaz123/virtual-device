#!/usr/bin/env python3
# coding: utf-8

import os
import socket
import struct
from . import config


class Sock(object):

    def __init__(self):
        self.s = None
        self._create_socket()
        self._wait_client()

    def _create_socket(self):
        self.s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        if os.path.exists(config.socket_file):
            os.unlink(config.socket_file)

        self.s.bind(config.socket_file)
        self.s.listen(1)

    def _wait_client(self):
        print('Waiting the client...')
        self.conn = self.s.accept()[0]
        print('Connection established')

    def send(self, model, dev_id, data):
        """Send data to client, it will encapsulate header in data
        automatically
        """
        # check data validate
        if type(data is not bytes):
            raise ValueError('data type must be bytes')

        # TODO: encapsulate

        self.conn.send(data)

    def recv(self):
        """Receive data from client, decapsulate header automatically
        """
        # refer to posix_message_header_t at posix_manager.c
        header = self.conn.recv(12)
        if not header:
            raise ValueError('header not exist')

        # decapsulate header
        model, dev_id, length = struct.unpack('III', header)

        # receive data
        data = self.conn.recv(length)

        return model, dev_id, data
