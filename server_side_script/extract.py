import cv2,sys
import numpy as np

vid = cv2.VideoCapture("test.avi")
iters=0
while(True):
    iters+=1
    # Capture the video frame
    # by frame
    ret, frame = vid.read()
    img= frame #[:800,:1200]
    frame1= frame #[:800,:1200]
    #print(frame1)

    g=int(sys.argv[1])
    if(iters):
        frame1 = cv2.flip(frame1,0)
        frame1 = cv2.flip(frame1,1)
        cv2.imwrite("original"+str(iters)+".jpg",frame1)
        inBlack  = np.array([10,10,10], dtype=np.float32)
        inWhite  = np.array([190, 255, 190], dtype=np.float32)
        inGamma  = np.array([g, g, g], dtype=np.float32)
        outBlack = np.array([0, 0, 0], dtype=np.float32)
        outWhite = np.array([500, 500,500], dtype=np.float32)
        # Display the resulting frame
        img = np.clip( (frame1 - inBlack) / (inWhite - inBlack), 0, 255 )                            
        img = ( img ** (1/inGamma) ) *  (outWhite - outBlack) + outBlack
        img = np.clip( img, 0, 255).astype(np.uint8)
        img=cv2.medianBlur(img,5)
        cv2.imwrite("color"+str(iters)+".jpg",img)
#        img = cv2.fastNlMeansDenoisingColored(img,None,7,7,5,21)  
        print(iters)
        fr=cv2.resize(img,(700,700))
        cv2.imshow('frame', fr)
        cv2.imwrite("color_corrected"+str(iters)+".jpg",img)
        # the 'q' button is set as the
        # quitting button you may use any
        # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
