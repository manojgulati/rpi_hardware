import os
import time
import paho.mqtt.client as mqtt
import math
import base64
import json
import random,string
import sys
# The callback for when the client receives a CONNACK response from the server.
counter =0
def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))
def on_message(client, userdata, msg):
        print "received"
        global counter
        counter+=1
        #print(msg.topic+" "+str(msg.payload))
        decoded = base64.b64decode(msg.payload)
        with open("test.jpg","wb") as im:
            im.write(decoded)
def convertImageToBase64():
	with open("/run/user/1000/images/m.jpg", "rb") as image_file:
		encoded = base64.b64encode(image_file.read())
		return encoded
def randomword(length):
	return ''.join(random.choice(string.lowercase) for i in range(length))
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("127.0.0.1", 1883, 60)
client.subscribe("apple")
client.loop_forever()
