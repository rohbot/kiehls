import serial
import time
import os
import redis
import requests

strPort = '/dev/ttyACM0'

ser = serial.Serial(strPort, 9600)

#redis = redis.Redis(password="redisb0x1337")

#redis_cloud = redis.Redis(host='rohbot', password="redisb0x1337")

#redis_pi = redis.Redis(host='raspi')

while 1:
	line = ser.readline()
	print line

	#os.system('xdotool key XF86AudioPlay')
	#redis_cloud.publish('fb-out', "arduino press " + str(count))
	#redis_pi.publish('socketio', 'blink')
	#redis_pi.publish('socketio', 'button1')
	
	#os.system('xdotool key XF86AudioNext')
	