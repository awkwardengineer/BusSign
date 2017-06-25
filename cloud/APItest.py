#import urllib2
from bs4 import BeautifulSoup
import socket
import os.path
import requests

device = "470053000b51353432383931"
baseUrl = "https://api.particle.io/v1/devices/"
arg = "010s020s"
func = "setBusTimes"

url = baseUrl + device + "/" + func

with open(os.path.expanduser('~/Dropbox/Keys/particletoken'), "r") as f:
    token = f.read().rstrip()
    print token

payload = {'arg':arg, 'access_token':token}

r = requests.post(url, data = payload)

print r.url
print r.text

"""
def isInternetUp(self,host="8.8.8.8",port=53,timeout=1):
	try:
		socket.setdefaulttimeout(timeout)
		socket.socket(socket.AF_INET,socket.SOCK_STREAM).connect((host,port))
		return True
	except Exception as ex:
		print ex.message
		print "No Instanets"
		return False
"""
