import os
from time import sleep
import sys
server_no = sys.argv[1]
folder = sys.argv[2]

while 1:
    os.system("python3 server.py "+server_no+" "+ folder)
    sleep(0.5)
