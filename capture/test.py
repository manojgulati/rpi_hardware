import os
from time import sleep
import socket
from time import time
dur = os.path.getmtime('test.avi')
dur2 = os.path.getmtime('test2.avi')
count=0
las=0
las2=0
UDP_PORT = 5003
UDP_IP = '192.168.86.112'
n=0
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
def send_chuncks(data,lent):
    global n
    n+=1
    i=-65000
    for i in range(0,lent - (lent%65000),65000):
        dt = data[i:i+65000]
        sock.sendto(dt,(UDP_IP, UDP_PORT))
        n+=1
    if((lent%65000) != 0):
        dt = data[i+65000:]
        sock.sendto(dt,(UDP_IP, UDP_PORT))
        n+=1
lenss = 0
while True:
    t = time()
    if (dur != os.path.getmtime('test.avi')):
        print("avi1, updated")
        dur = os.path.getmtime('test.avi')
        f = open("test.avi")
        f.seek(las)
        l = f.read()
        las=f.tell()
        send_chuncks(l,len(l)) 
        lenss+=len(l)
        f.close()
    if (dur2 != os.path.getmtime('test2.avi')):
        print("avi2,updated")
        dur2 = os.path.getmtime('test2.avi')
        f = open("test2.avi")
        f.seek(las2)
        l = f.read()
        las2=f.tell()
        send_chuncks(l,len(l)) 
        lenss+=len(l)
        f.close()
    delay = 0.20 - (time()-t)
    if(delay<0):
        delay = 0
    sleep(delay)
