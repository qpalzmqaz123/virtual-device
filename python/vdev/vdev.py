#!/usr/bin/env python3
# coding: utf-8

import sys
import logging
from multiprocessing import Process, Queue
from threading import Thread

from .sock import Sock


class DeviceProcess(Process):

    def __init__(self, dev_class, queue, model, dev_id, send_queue):
        self.cls = dev_class
        self.ins = self.cls()
        self.q = queue
        self.model = model
        self.dev_id = dev_id
        self.send_q = send_queue
        super().__init__()

    def run(self):
        # save queue, model, id
        self.ins.set_send_options(self.send_q, self.model, self.dev_id)

        # create a new thread to call instance's run()
        t = Thread(target=self.cls.run, args=(self.ins,))
        t.start()

        # loop queue
        while True:
            data = self.q.get()
            self.ins.received(data)


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
        # key => {'queue': Queue, 'class': type, 'model': int, 'id': int}
        self.table = {}
        self.s = None
        self.procs = []
        super().__init__()

    def _gen_key(self, model, dev_id):
        """Generate hash key
        """
        return '%d-%d' % (model, dev_id)

    def register_device(self, dev_class, model, device_id=0):
        """Register virtual device.

        Parameters:
            dev_class:    device class
            model:        device model number, refer to vdev/inc/vdev_model.h
            device_id:    available when multi-device exist
        """
        key = self._gen_key(model, device_id)

        self.table[key] = {
            'class': dev_class,
            'queue': Queue(),
            'model': model,
            'id': device_id
        }

    def run(self):
        """Loop the socket forever, then distribute the data to each 
        callback via model and device id 
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
            proc = DeviceProcess(info['class'], info['queue'], info['model'], info['id'], send_queue)
            self.procs.append(proc)
        [proc.start() for proc in self.procs]

        while True:
            try:
                model, dev_id, data = self.s.recv()
            except ValueError as e:
                continue

            # look up queue by model and dev_id
            key = self._gen_key(model, dev_id)
            info = self.table.get(key)
            if info == None:
                logging.error('device %d-%d not fond!' % (model, dev_id))
                continue

            # distribute data
            info['queue'].put(data)
