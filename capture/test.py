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
    if (dur != os.path.getmtime(file1)):
#        print("avi1, updated")
        dur = os.path.getmtime(file1)
        f = open(file1)
        f.seek(las)
        l = f.read()
        las=f.tell()
        send_chuncks(l,len(l)) 
        lenss+=len(l)
        f.close()
    if (dur2 != os.path.getmtime(file2)):
#        print("avi2,updated")
        dur2 = os.path.getmtime(file2)
        f = open(file2)
        f.seek(las2)
        l = f.read()
        las2=f.tell()
        send_chuncks(l,len(l)) 
        lenss+=len(l)
        f.close()
    delay =0.20 - (time()-t)
    if(delay<0):
        delay = 0
    sleep(delay)
