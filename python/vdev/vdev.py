#!/usr/bin/env python3
# coding: utf-8

import sys
import logging
from multiprocessing import Process, Queue
from threading import Thread

from .sock import Sock


class DeviceProcess(Process):

    def __init__(self, dev_class, queue, model, send_queue):
        self.cls = dev_class
        self.ins = self.cls()
        self.recv_q = queue
        self.model = model
        self.send_q = send_queue

        super().__init__()

    def run(self):
        # save queue, model
        self.ins.set_send_options(self.recv_q, self.send_q, self.model)

        # loop queue
        self.ins.recv_loop()


class SendThread(Thread):

    def __init__(self, queue, sock_session):
        self.q = queue
        self.s = sock_session

        super().__init__()

    def run(self):
        while True:
            obj = self.q.get()
            self.s.send(obj['model'], obj['id'], obj['data'])


class Vdev(Process):

    def __init__(self):
        # key => {'queue': Queue, 'class': type, 'model': int}
        self.table = {}
        # socket
        self.s = None
        # device process array
        self.procs = []

        super().__init__()

    def register_device(self, dev_class, model):
        """Register virtual device.

        Parameters:
            dev_class:    device class
            model:        device model number, refer to vdev/inc/vdev_model.h
        """
        self.table[model] = {
            'class': dev_class,
            'queue': Queue(),
            'model': model,
        }

    def run(self):
        """Loop the socket forever, then distribute the data to each 
        callback via model number.
        """
        # create socket
        self.s = Sock()

        # create a queue for send data
        send_queue = Queue()

        # create thread which loop send_queue and send data to client
        send_thread = SendThread(send_queue, self.s)
        send_thread.start()

        # start all process
        for info in self.table.values():
            proc = DeviceProcess(info['class'], info['queue'], info['model'], send_queue)
            self.procs.append(proc)
        [proc.start() for proc in self.procs]

        while True:
            try:
                model, dev_id, data = self.s.recv()
            except ValueError as e:
                continue

            # look up queue by model number
            info = self.table.get(model)
            if info == None:
                logging.error('device %d-%d not fond!' % (model, dev_id))
                continue

            # distribute data
            info['queue'].put(dict(data=data, dev_id=dev_id))

