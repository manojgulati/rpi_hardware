import cv2
import numpy as np
import sys,os
vid_devs=[]
for i in range(len(sys.argv)-2):
    vid_devs.append(sys.argv[i+2])
iters=0
g=float(sys.argv[1])
seqs=open("seq.txt")
frame_nos = seqs.readlines()
cur_frame=1
out_folder="outputs"
os.system("rm -rf "+"outputs")
os.system("mkdir "+"outputs")
for fr_no in frame_nos:
    iters = int(fr_no)
    # Capture the video frame
    # by frame
    imgs=[]
    for vid in vid_devs:
        img_name =vid+str(cur_frame)+".jpg"
        print(img_name)
        frame = cv2.imread(img_name)
        img= frame #[:800,:1200]
        frame1= frame #[:800,:1200]
        frame1 = cv2.flip(frame1,0)
        frame1 = cv2.flip(frame1,1)
        inBlack  = np.array([10,10,10], dtype=np.float32)
        inWhite  = np.array([190, 255, 190], dtype=np.float32)
        inGamma  = np.array([g, g, g], dtype=np.float32)
        outBlack = np.array([0, 0, 0], dtype=np.float32)
        outWhite = np.array([500, 500,500], dtype=np.float32)
        # Display the resulting frame
        img = np.clip( (frame1 - inBlack) / (inWhite - inBlack), 0, 255 )                            
        img = ( img ** (1/inGamma) ) *  (outWhite - outBlack) + outBlack
        img = np.clip( img, 0, 255).astype(np.uint8)
        (height,width,_) = img.shape
        scale=int(1080/height)
#        img=cv2.medianBlur(img,5)
        if(scale==10):
            img = cv2.fastNlMeansDenoisingColored(img,None,10,10,7,21)  
        hh= height*scale
        ww= width*scale
        print(hh,ww,height,width,scale)
        img1=cv2.resize(img,(ww,hh),interpolation=cv2.INTER_CUBIC)
        imgs.append(img1)
    if(len(imgs)>1):
        final = np.concatenate(tuple(imgs), axis=1)
    else:
        final = imgs[0]
    cv2.imwrite(out_folder+"/"+str(iters)+".jpg",final)
    print(cur_frame)
    cv2.imshow("frame", final)
    cur_frame+=1
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
