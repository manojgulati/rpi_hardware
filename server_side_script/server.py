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
    inBlack  = np.array([0, 0, 0], dtype=np.float32)
    inWhite  = np.array([205, 255, 205], dtype=np.float32)
    inGamma  = np.array([g, g, g], dtype=np.float32)
    outBlack = np.array([0, 0, 0], dtype=np.float32)
    outWhite = np.array([255, 255, 255], dtype=np.float32)
    # Display the resulting frame
    img = np.clip( (frame - inBlack) / (inWhite - inBlack), 0, 255 )                            
    img = ( img ** (1/inGamma) ) *  (outWhite - outBlack) + outBlack
    img = np.clip( img, 0, 255).astype(np.uint8)
    img = cv2.flip(img,0)
    img = cv2.flip(img,1)
    #img = cv2.fastNlMeansDenoisingColored(img,None,3,5,7,21)  
    #img = cv2.medianBlur(img,5)
    return img 
seq=[]
seq_file=open("seq.txt","w")
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
            sequence= sequence.splitlines()
            for ss in sequence:
                seq.append(int(ss))
                seq_file.write(str(int(ss))+"\n")
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
                if(writer1_open==0):
                    video1 = cv2.VideoWriter(folder+'/test.avi',fourcc, fps, (width,height))
                    writer1_open=1
                #cv2.putText(img,'frame '+str(frame1), bottomLeftCornerOfText,font,fontScale,fontColor,lineType)
                #print(frame1,str(seq[frame1-1]))
                #cv2.imwrite(folder+'/frame1_'+str(seq[frame1-1])+'.jpg',img1)
                print(TCP_PORT," frame1_no ",frame1)
                video1.write(img1)
                scale=(1080/height)
                hh= height*scale
                ww= width*scale
                hh1 = int(hh/1080*600)
                ww1 = int(ww/1080*600)
                smal=cv2.resize(img1,(ww1,hh1))
                #print("fram1",frame1)
                cv2.imshow("cam1",smal)
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
                if(writer2_open==0):
                    video2 = cv2.VideoWriter(folder+'/test2.avi',fourcc, fps, (width,height))
                    writer2_open=1
                #img = process(img)
                img1 = img[:,:]
                #print(frame1,str(seq[frame2-1]))
                scale=(1080/height)
                hh= height*scale
                ww= width*scale
                hh1 = int(hh/1080*600)
                ww1 = int(ww/1080*600)
                smal=cv2.resize(img1,(ww1,hh1))
                #cv2.imwrite(folder+'/frame2_'+str(seq[frame2-1])+'.jpg',img1)
                video2.write(img1)
                #smal=cv2.resize(img,(600,600))
                #print("fram1",frame1)
                cv2.imshow("cam2",smal)
                cv2.waitKey(3)
    else:
        print("Breaking..")
        break
s.close()
