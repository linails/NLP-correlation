#! /usr/bin/python
# -*- coding: UTF-8 -*-
# Created Time: 2017-03-08 20:58:41
# Last modified: 2017-03-08 22:55:22
# 

import socket
import time
import threading
import sys

__metaclass__ = type
class Connection:

    def __init__(self, mutex_data, data, para):
        print "connection init "
        self.threads    = []
        self.threads.append(threading.Thread(target=self.rw_loop))
        self.buff       = []
        self.para       = para
        try :
            self.client     = socket.socket()
            self.host   = socket.gethostname()
            self.port   = 9190

            self.client.connect((self.host, self.port))

            for tds in self.threads:
                tds.start()

        except socket.error, e:
            print "connect error : %s" % e
            self.client.close()
            self.para[0] = False
            sys.exit(1)

    def rw_loop(self):
        try :
            times = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
            for i in times:
                print "i = ", i
                time.sleep(1)
                self.client.send("recv data ok !")
                self.buff = self.client.recv(1024)
                print "buff = ", self.buff
        except socket.error, e:
            print "rw_loop conn error : %s" % e
            self.para[0] = False
            sys.exit(1)

    def uninit(self):
        print "close client "
        self.client.close()

        for tds in self.threads:
            tds.join()
            print ""
            print "tds :", tds
            print ""


