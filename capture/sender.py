import os
from time import sleep
import socket
from time import time
file1='test.avi'
file2='test2.avi'
dur = os.path.getmtime(file1)
dur2 = os.path.getmtime(file2)
count=0
las=0
las2=0
TCP_PORT = 5003
TCP_IP = '192.168.86.54'
n=0
sock = socket.socket()
sock.connect((TCP_IP, TCP_PORT))
def send_chuncks(data,lent):
        if(lent>0):
            size = str(len(data)).ljust(16).encode('utf-8')
            print(size)
            sock.send(size)
            sock.send(data)
lenss = 0
while True:
    t = time()
    if (dur != os.path.getmtime(file1)):
        dur = os.path.getmtime(file1)
        f = open(file1)
        f.seek(las)
        l = f.read()
        las=f.tell()
        send_chuncks(l,len(l)) 
        lenss+=len(l)
        f.close()
    if (dur2 != os.path.getmtime(file2)):
        dur2 = os.path.getmtime(file2)
        f = open(file2)
        f.seek(las2)
        l = f.read()
        las2=f.tell()
        #send_chuncks(l,len(l)) 
        lenss+=len(l)
        f.close()
    delay =0.2 - (time()-t)
    if(delay<0):
        delay = 0
    sleep(delay)