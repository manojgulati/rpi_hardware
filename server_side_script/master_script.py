#!/usr/bin/python
import sys,os
os.system("pkill -f server.py")
try:
    no_of_clients=int(sys.argv[1])
except:
    try:
        no_of_clients=int(input("input number of servers needed : "))
    except:
        sys.exit(0)
for serv_no in range(no_of_clients):
    folder_name = "images_server_"+str(serv_no+1)
    os.system("fuser -k "+str(5000+serv_no+1)+"/tcp")
    os.system("rm -rf "+folder_name)
    os.system("mkdir "+folder_name)
    os.system("python loop_server.py "+str(serv_no +1)+" "+folder_name+" &")
try:
    while(1):
        pass
except:
    print("killing the servers")
    os.system("pkill -f server.py")
