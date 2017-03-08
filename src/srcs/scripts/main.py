#! /usr/bin/python
# -*- coding: UTF-8 -*-
# Created Time: 2017-03-08 21:02:02
# Last modified: 2017-03-08 22:56:20
# 

from pprint import pprint
import time
import threading
import connection

__metaclass__ = type
class MachineLearning:

    def __init__(self):
        print "python module init !"
        self.threads    = []
        self.data       = []
        self.para       = [True, []]
        
        self.mutex_data = threading.Lock()
        self.threads.append(threading.Thread(target=self.worker_calc))

        self.conn = connection.Connection(self.mutex_data, self.data, self.para)

        for tds in self.threads:
            tds.start()

    def worker_calc(self):
        print ""
        times = 10
        while True:
            if False == self.para[0]:
                print "socker error break !"
                break
            print "worker_calc ..."
            time.sleep(1)
            times -= 1
            if times <= 0:
                break

    def uninit(self):
        for tds in self.threads:
            tds.join()
            print ""
            print "tds :", tds
            print ""

        self.conn.uninit()

        print ""
        print "uninit() ..."
        print ""

if __name__ == '__main__':

    ml = MachineLearning()

    ml.uninit()

