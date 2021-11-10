import os,sys
from time import sleep
try:
    client_no = int(sys.argv[1])
except:
    try:
        client_no = int(input("enter the client number "))
    except:
        sys.exit(0)
try:
    TCP_IP = sys.argv[2]
    while 1:
        os.system("python mod_sender.py "+str(client_no)+" "+TCP_IP)
        sleep(0.5)
except:
    while 1:
        os.system("python mod_sender.py "+str(client_no))
        sleep(0.5)
