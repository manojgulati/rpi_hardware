#!/usr/bin/python
import socket
import cv2
import numpy as np
import os,sys
f= open('test.avi','wb')
folder = sys.argv[2]
def recvall(sock, count):
    buf = b''
    while count:
        newbuf = sock.recv(count)
        if not newbuf: return None
        buf += newbuf
        count -= len(newbuf)
    return buf

TCP_IP = ''
TCP_PORT = int(sys.argv[1])+5000
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(True)
print("Running server on port number "+str(TCP_PORT)+" server number "+ str(sys.argv[1]))
conn, addr = s.accept()
cur= 1
datas=[]
buff1=b''
buff2=b''
frame1=0
frame2=0
g=1.5
fps=8
fourcc=cv2.VideoWriter_fourcc(*'MJPG')
writer1_open=0
writer2_open=0
def process(frame):
    return frame
seq=[]
seq_file=open(folder+"/seq.txt","w")
seq_file.close()
while(1):
    if 1:
        hit=1
        font                   = cv2.FONT_HERSHEY_SIMPLEX
        bottomLeftCornerOfText = (30,30)
        fontScale              = 0.5
        fontColor              = (255,255,255)
        lineType               = 2

        length = recvall(conn,16)
        stringData = recvall(conn, int(length))
        if(stringData[0]==49):
            buff1+=stringData[1:]
        if(stringData[0]==50):
            buff2+=stringData[1:]
        if(stringData[0]==51):
            sequence= stringData[1:]
            seq_file=open(folder+"/seq.txt","a")
            seq_file.write(sequence.decode("utf-8"))
            seq_file.close()
        while(hit==1):
            hit=0
            a = buff1.find(b'\xff\xd8')
            b = buff1.find(b'\xff\xd9')
            if(a!=-1 and b!=-1):
                hit=1
                frame1+=1
                #print(a,b)
                jpg = buff1[a:b+2]
                buff1=buff1[b+2:]
                ff=np.fromstring(jpg, dtype='uint8')
                #print(ff)
                img = cv2.imdecode(ff,cv2.IMREAD_COLOR)
                print(img.shape)
                (height,width,_) = img.shape
                #img = process(img)
                img1 = img[:,:]
                #cv2.putText(img,'frame '+str(frame1), bottomLeftCornerOfText,font,fontScale,fontColor,lineType)
                #print(frame1,str(seq[frame1-1]))
                cv2.imwrite(folder+'/frame1_'+str(frame1)+'.jpg',img1)
                print(TCP_PORT," frame1_no ",frame1)
               # video1.write(img1)
                scale=(1080/height)
                hh= height*scale
                ww= width*scale
                hh1 = int(hh/1080*600)
                ww1 = int(ww/1080*600)
                smal=cv2.resize(img1,(ww1,hh1))
                #print("fram1",frame1)
                cv2.imshow("cam "+str(TCP_PORT-5000),smal)
                cv2.waitKey(3)
            a = buff2.find(b'\xff\xd8')
            b = buff2.find(b'\xff\xd9')
            if(a!=-1 and b!=-1):
                hit=1
                frame2+=1
                #print(a,b)
                jpg = buff2[a:b+2]
                buff2=buff2[b+2:]
                ff=np.fromstring(jpg, dtype='uint8')
                print(TCP_PORT," frame2_no ",frame2)
                #print(ff)
                img = cv2.imdecode(ff,cv2.IMREAD_COLOR)
                #print(img.shape)
                img1 = img[:,:]
                print(img.shape)
                (height,width,_) = img1.shape
                #img = process(img)
                img1 = img[:,:]
                #video2.write(img1)
                #print(frame1,str(seq[frame2-1]))
                scale=(1080/height)
                hh= height*scale
                ww= width*scale
                hh1 = int(hh/1080*600)
                ww1 = int(ww/1080*600)
                smal=cv2.resize(img1,(ww1,hh1))
                cv2.imwrite(folder+'/frame2_'+str(frame2)+'.jpg',img1)
                #smal=cv2.resize(img,(600,600))
                #print("fram1",frame1)
                cv2.imshow("cam2",smal)
                cv2.waitKey(3)
    else:
        print("Breaking..")
        break
s.close()
