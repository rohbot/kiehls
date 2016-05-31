from flask import Flask, request, render_template
import redis

redis = redis.Redis()


app = Flask(__name__)

redis.set('cans', 0)
redis.set('bottles', 0)

@app.route("/")
def hello():
	cans = int(redis.get('cans'))
	bottles = int(redis.get('bottles'))
	total = cans + bottles
	return render_template('index.html',cans=cans, bottles=bottles, total=total) 

@app.route("/counter/ping/<name>")
def cans(name):
	redis.incr(name)
	#print request.data
	return str(redis.incr('cans'))


@app.route("/bottles")
def bottles():
	#print request.data
	return str(redis.incr('bottles'))


if __name__ == "__main__":
    app.run(host="0.0.0.0",debug=True)