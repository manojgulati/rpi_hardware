#!/usr/bin/python
import socket
import cv2
import numpy
f= open('test.avi','wb')
def recvall(sock, count):
    buf = b''
    while count:
        newbuf = sock.recv(count)
        if not newbuf: return None
        buf += newbuf
        count -= len(newbuf)
    return buf

TCP_IP = ''
TCP_PORT = 5003

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(True)
conn, addr = s.accept()
cur= 1
datas=[]
buff1=b''
buff2=b''
frame1=0
frame2=0
while(1):
    try:
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
        a = buff1.find(b'\xff\xd8')
        b = buff1.find(b'\xff\xd9')
        if(a!=-1 and b!=-1):
            frame1+=1
            #print(a,b)
            jpg = buff1[a:b+2]
            buff1=buff1[b+2:]
            ff=numpy.fromstring(jpg, dtype='uint8')
            #print(ff)
            img = cv2.imdecode(ff,cv2.IMREAD_COLOR)
            #print(img.shape)
            cv2.putText(img,'frame '+str(frame1), bottomLeftCornerOfText,font,fontScale,fontColor,lineType)
            cv2.imshow("feed 1",img)
            cv2.waitKey(3)
        a = buff2.find(b'\xff\xd8')
        b = buff2.find(b'\xff\xd9')
        if(a!=-1 and b!=-1):
            frame2+=1
            jpg = buff2[a:b+2]
            buff2=buff2[b+2:]
            img = cv2.imdecode(numpy.fromstring(jpg, dtype='uint8'),cv2.IMREAD_COLOR)
            cv2.putText(img,'frame '+str(frame2), bottomLeftCornerOfText,font,fontScale,fontColor,lineType)
            cv2.imshow("feed 2",img)
            cv2.waitKey(3)
    except:
        print("Breaking..")
        break
s.close()
