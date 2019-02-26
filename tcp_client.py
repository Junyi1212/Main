#!usr/bin/env python
# -*- coding: utf8 -*-
import socket

if __name__ == "__main__":
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('127.0.0.1', 12280))
    print(s.recv(1024).decode('utf-8'))
    s.close()