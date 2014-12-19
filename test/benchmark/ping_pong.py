#!/usr/bin/env python

from socket import *
from time import *
from sys import *

addr = "cp01-rdqa-pool645.cp01.baidu.com"
port = 12345

def runner (max_count = 10000):
    global addr, port
    count = cost = 0
    s = socket(AF_INET, SOCK_STREAM)
    s.connect((addr, port))
    while count < max_count:
        count += 1
        now = time()
        s.send("ping")
        result = s.recv (1024)
        if result == "ping":
             cost += (time() - now) * 1000
        else:
            print "some error happens"
            exit (0)

    return count, cost


import threading
t = threading.Thread(target = runner)
result = t.run()
print result


    
