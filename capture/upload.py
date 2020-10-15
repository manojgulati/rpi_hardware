import os
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

def convertImageToBase64():
	with open("/run/user/1000/images/m.jpg", "rb") as image_file:
		encoded = base64.b64encode(image_file.read())
		return encoded
def randomword(length):
	return ''.join(random.choice(string.lowercase) for i in range(length))
packet_size=3000

def publishEncodedImage(encoded):
	end = packet_size
	start = 0
	length = len(encoded)
	picId = randomword(8)
	pos = 0
	no_of_packets = math.ceil(length/packet_size)
	while start <= len(encoded):
		data = {"data": encoded[start:end], "pic_id":picId, "pos": pos, "size": no_of_packets}
		client.publish("apple", json.JSONEncoder().encode(data))
		end += packet_size
		start += packet_size
		pos = pos +1
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("192.168.1.38", 1883, 60)
dur = os.path.getmtime('/run/user/1000/images/test.txt')
count=0
while True:
	if (dur != os.path.getmtime('/run/user/1000/images/test.txt')):
		dur = os.path.getmtime('/run/user/1000/images/test.txt')
		encoded = convertImageToBase64()
		publishEncodedImage(encoded)
		client.publish("apple","file updated "+str(count))
		count+=1
