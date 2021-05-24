import os
from time import sleep
import socket
from time import time
import psutil
file1='test.avi'
file2='test2.avi'
TCP_PORT = 5002
TCP_IP = '192.168.18.13'
n=0
sock = socket.socket()
sock.connect((TCP_IP, TCP_PORT))
def send_chuncks(data,lent):
        if(lent>0):
            size = str(len(data)).ljust(16).encode('utf-8')
            sock.send(size)
            sock.send(data)
lenss = 0
started=0
im_no=0
im_no2=0
while True:
    t = time()
    if((started==0) and os.path.exists("running.re")):
        print("started")
        started=1
        pass
    if (started):
        if(os.path.exists("./images/m"+str(im_no+1)+".ppm")):
            f = open("./images/m"+str(im_no)+".ppm")
            l = b'1'+f.read()
            send_chuncks(l,len(l)) 
            f.close()
            im_no+=1
        if(os.path.exists("./images/n"+str(im_no2+1)+".ppm")):
            f = open("./images/n"+str(im_no)+".ppm")
            l = b'2'+f.read()
            send_chuncks(l,len(l)) 
            f.close()
            im_no2+=1
    if(started and os.path.exists("running.re")):
        pass
    elif(started):
        print("not running")
        sock.close()
        break
    delay =0.050 - (time()-t)
    if(delay<0):
   #     print(delay)
        delay = 0
    sleep(delay)
