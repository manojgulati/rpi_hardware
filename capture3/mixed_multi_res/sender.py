import os,glob,sys
from time import sleep
import numpy as np
import socket
import time
import psutil
import cv2
TCP_PORT = 5003
TCP_IP = '192.168.147.148'
n=0
sock = socket.socket()
sock.connect((TCP_IP, TCP_PORT))
def send_chunks(data,lent):
        if(lent>0):
            size = str(len(data)).ljust(16).encode('utf-8')
            sock.send(size)
            sock.send(data)
path=sys.argv[1]
os.chdir(path)
dat=[]
for file in glob.glob("*.avi"):
    cap= open(file,'rb') 
    bytes=b''
    while True:
        bytes += cap.read(1024)
        a = bytes.find(b'\xff\xd8')
        b = bytes.find(b'\xff\xd9')
        if(a!=-1 and b!=-1):
            jpg = bytes[a:b+2] 
            bytes = bytes[b+2:]
            dat.append(jpg)
            break
true= True
while true:
    t=time.time()
    for data in dat:
        send_chunks(data,len(data))
    d= time.time()-t
    if(d>0.125):
        print("neg",d)
        d=0.125 
    sleep(0.125-d)
#while True:;
#    t = time()
#    if (dur != os.path.getmtime(file1)):
#        started=1
#        dur = os.path.getmtime(file1)
#        f = open(file1)
#        f.seek(las)
#        l = b'1'+f.read()
#        las=f.tell()
#        send_chuncks(l,len(l)) 
#        lenss+=len(l)
#        f.close()
