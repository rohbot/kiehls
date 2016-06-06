import serial
import time
import os
import redis
import requests
import urllib2
import sys
strPort = '/dev/ttyACM0'

ser = serial.Serial(strPort, 9600)

redis = redis.Redis()

name = redis.get('name')
print 'I am ' + name
#redis_cloud = redis.Redis(host='rohbot', password="redisb0x1337")

#redis_pi = redis.Redis(host='raspi')
#URL = "http://localhost:5000/counter/ping/"
#URL = "http://128.199.67.134:5000/counter/ping/"
URL = "https://abetterearth.com.my:5000/counter/ping/"


def internet_on():
    try:
        response=urllib2.urlopen('http://52.87.228.17:8080/alive/' + name,timeout=60)
        return True
    except urllib2.URLError as err: pass
    return False

def httpPost(target):

	r = requests.get(URL + target)
	print r.text

internet_on()

while 1:
	try:
		line = ser.readline()
		vals = line.strip().split(' ')
		sensor = int(vals[0])
		count = int(vals[1])

		#print sensor, count
		if count != 0:

			if sensor == 0:
				print line
			if sensor == 2:
				print 'Bottle up!' , count
				#httpPost(name)
			if sensor == 1:
				print 'Can Can'	, count
				httpPost(name)
	except KeyboardInterrupt:
		raise
	except:
		e = sys.exc_info()[0]

		print "something messed up", e				
	#os.system('xdotool key XF86AudioPlay')
	#redis_cloud.publish('fb-out', "arduino press " + str(count))
	#redis_pi.publish('socketio', 'blink')
	#redis_pi.publish('socketio', 'button1')
	
	#os.system('xdotool key XF86AudioNext')
	
