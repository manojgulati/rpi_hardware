import os
import sys
import time
import paho.mqtt.client as mqtt
import math
import base64
import json
import random,string
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))
	# Subscribing in on_connect() means that if we lose the connection and
	# reconnect then subscriptions will be renewed.
	client.subscribe("$SYS/#")
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	print(msg.topic+" "+str(msg.payload))
resx = [ 1200, 1100, 900, 900, 800, 700,  600, 600, 512 , 440, 360, 224, 160, 96, 70]
resy = [ 800,  800,  900, 800, 800, 800 , 800, 600, 512 , 440, 360, 224, 160, 96, 70]
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("192.168.137.210", 1883, 60)
print "connected"
while(1):
	pass
def publishEncodedImage(encoded):
	client.publish("apple",encoded)
res = int(sys.argv[1])
if 1:
	val = ""
	chunk_size = 160*160
	payload_size= resx[res]*resy[res]
	print payload_size,chunk_size,payload_size/chunk_size
	lists = []
	for pix in range(payload_size):
		val+=chr(pix%100+65)
	for i in range(int(sys.argv[2])):
		iteri=-1
		start_time= time.time()
		for iteri in range(payload_size/chunk_size):
			publishEncodedImage(val[chunk_size*iteri:chunk_size*(iteri+1)])
		if((payload_size%chunk_size)!= 0):
			publishEncodedImage(val[chunk_size*(iteri+1):-1])
		lists.append(time.time()-start_time)
		publishEncodedImage("Done_man")
	print "resolution", "time", sum(lists)/len(lists)
