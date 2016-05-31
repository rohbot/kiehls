import serial
import time
import os
import redis
import requests

strPort = '/dev/ttyACM0'

ser = serial.Serial(strPort, 9600)

redis = redis.Redis()

name = redis.get('name')
print 'I am ' + name
#redis_cloud = redis.Redis(host='rohbot', password="redisb0x1337")

#redis_pi = redis.Redis(host='raspi')
#URL = "http://localhost:5000/counter/ping/"
URL = "http://128.199.67.134:5000/counter/ping/"

def httpPost(target):

	r = requests.get(URL + target)
	print r.text

while 1:
	line = ser.readline()
	vals = line.strip().split(' ')
	sensor = int(vals[0])
	count = int(vals[1])

	#print sensor, count
	if sensor == 0:
		print line
	if sensor == 2:
		print 'Bottle up!' , count
		#httpPost(name)
	if sensor == 1:
		print 'Can Can'	, count
		httpPost(name)

	#os.system('xdotool key XF86AudioPlay')
	#redis_cloud.publish('fb-out', "arduino press " + str(count))
	#redis_pi.publish('socketio', 'blink')
	#redis_pi.publish('socketio', 'button1')
	
	#os.system('xdotool key XF86AudioNext')
	