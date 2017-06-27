import urllib2
from bs4 import BeautifulSoup
import socket
import time
import math
import requests
import os.path

device = "470053000b51353432383931"
baseUrl = "https://api.particle.io/v1/devices/"
arg = "010s020s"
func = "setBusTimes"

url = baseUrl + device + "/" + func

with open(os.path.expanduser('~/Dropbox/Keys/particletoken'), "r") as f:
    token = f.read().rstrip()
    print token

class BusSignCloud:
	def __init__(self):

		self.Fenwick89=[]
		self.Thurston89101=[]
		self.Malden101=[]
		self.Medford80=[]
		self.commandString= ""

	def isInternetUp(self,host="8.8.8.8",port=53,timeout=1):
		try:
			socket.setdefaulttimeout(timeout)
			socket.socket(socket.AF_INET,socket.SOCK_STREAM).connect((host,port))
			return True
		except Exception as ex:
			print ex.message
			print "No Instanets"
			return False

	def getFeed(self):
		if self.isInternetUp():
			try:
				raw_fenwick = urllib2.urlopen("http://webservices.nextbus.com/service/publicXMLFeed?command=predictions&a=mbta&stopId=2727")
				self.fenwick_soup = BeautifulSoup(raw_fenwick, "html.parser")

				raw_thurston = urllib2.urlopen("http://webservices.nextbus.com/service/publicXMLFeed?command=predictions&a=mbta&stopId=2704")
				self.thurston_soup = BeautifulSoup(raw_thurston, "html.parser")

				raw_medford = urllib2.urlopen("http://webservices.nextbus.com/service/publicXMLFeed?command=predictions&a=mbta&stopId=2387")
				self.medford_soup = BeautifulSoup(raw_medford, "html.parser")

				return True
			except urllib2.URLError as err:
				print type(err)
				print "Feed is Down"
				return False
			except socket.timeout as err:
				print type(err)
				print "Instanets is down"
				return False
			except:
				print "Unknown Error"

		else:
			return False

	def updateDisplay(self):

		self.commandString = ""

		#sort on first key, which is minutes
		self.Fenwick89.sort(key=lambda x: x[0])

		###Davis is the first line
		for datapoint in range (0,4):
			if (datapoint + 1) > len(self.Fenwick89):
				#a negative number turns off the display
				self.commandString += '-10s'
			else:
				if self.Fenwick89[datapoint][1]=="Clarendon":
					dot = True
				else:
					dot = False

				nextBusTime = self.Fenwick89[datapoint][0]
				self.commandString += self.intToCommand(nextBusTime);

		###The Malden bus is next
		for datapoint in range (0,4):
			if (datapoint + 1) > len(self.Malden101):
				#self.myseg.turnPairOff((0+datapoint)+pairOffset)
				self.commandString += '-10s'
			else:
				nextBusTime = self.Malden101[datapoint][0]
				self.commandString += self.intToCommand(nextBusTime);

		###The Sullivan bus is next
		for datapoint in range (0,4):

			if (datapoint + 1) > len(self.Thurston89101):
				#self.myseg.turnPairOff((0+datapoint)+pairOffset)
				self.commandString += '-10s'
			else:
				nextBusTime = self.Thurston89101[datapoint][0]
				self.commandString += self.intToCommand(nextBusTime);

		#end with the Lechmere bus.
		#I actually short it, and max it at 3 buses to avoid
		#overflowing the 64character buffer
		for datapoint in range (0,3):
			if (datapoint + 1) > len(self.Medford80):
				self.commandString += '-10s'
			else:
				nextBusTime = self.Medford80[datapoint][0]
				self.commandString += self.intToCommand(nextBusTime);






	def processFeed(self):

		self.Fenwick89=[]

		print " "
		print "Broadway @ Fenwick:"


		davis_buses = self.fenwick_soup.find(title='Clarendon Hill')
		if davis_buses != None:
			davis_buses = davis_buses.find_all('prediction')
			davis_buses = map(lambda x: x['seconds'], davis_buses)
			print "89 bus(es) to Davis via Clarendon Hill: " + ' '.join(davis_buses)
			for bus in davis_buses:
				self.Fenwick89.append([int(bus),"Clarendon"])

		davis_buses = self.fenwick_soup.find(title='Davis')
		if davis_buses != None:
			davis_buses = davis_buses.find_all('prediction')
			davis_buses = map(lambda x: x['seconds'], davis_buses)
			print "89 bus(es) to Davis: " + ' '.join(davis_buses)
			for bus in davis_buses:
				self.Fenwick89.append([int(bus),"Davis"])

		davis_buses = self.fenwick_soup.find(title='Clarendon Hill via Davis')
		if davis_buses != None:
			davis_buses = davis_buses.find_all('prediction')
			davis_buses = map(lambda x: x['seconds'], davis_buses)
			print "89 bus(es) to Davis: " + ' '.join(davis_buses)
			for bus in davis_buses:
				self.Fenwick89.append([int(bus),"Davis"])



		self.Malden101 = []
		malden_buses = self.fenwick_soup.find(title='Malden')
		if malden_buses != None:
			malden_buses = malden_buses.find_all('prediction')
			malden_buses = map(lambda x: x['seconds'], malden_buses)
			print "101 bus(es) to Malden: " + ' '.join(malden_buses)

			for bus in malden_buses:
				self.Malden101.append([int(bus),"Malden"])



		print " "
		print "Broadway @ Thurston:"

		self.Thurston89101 = []
		sullivan_buses = self.thurston_soup.find_all('prediction')
		if sullivan_buses != None:
			sullivan_buses = map(lambda x: x['seconds'], sullivan_buses)
			sullivan_buses = map(lambda x: int(x), sullivan_buses)
			sullivan_buses.sort()   #sorts in place
			sullivan_buses = map(lambda x: str(x), sullivan_buses)
			print "89/101 bus(es) to Sullivan: " + ' '.join(sullivan_buses)

			for bus in sullivan_buses:
				self.Thurston89101.append([int(bus),"Sullivan"])

		print " "
		print "Medford @ Thurston:"

		self.Medford80 = []
		lechmere_buses = self.medford_soup.find_all('prediction')
		if lechmere_buses != None:
			lechmere_buses = map(lambda x: x['seconds'], lechmere_buses)
			lechmere_buses = map(lambda x: int(x), lechmere_buses)
			lechmere_buses.sort()   #sorts in place
			lechmere_buses = map(lambda x: str(x), lechmere_buses)
			print "80 bus(es) to Lechmere: " + ' '.join(lechmere_buses)

			for bus in lechmere_buses:
				self.Medford80.append([int(bus),"Lechmere"])

		print " "
		print "===================================="
		print " "

	def intToCommand(self, number):
		tempString = ""
		if int(number) < 999:
			#make it a string
			tempString = str(number).zfill(3) + 's'
		else:
			tempString = int(math.floor(number/10))
			tempString = str(tempString).zfill(3)
			tempString += 'm'

		return tempString

	def sendToCloud(self):
		payload = {'arg':self.commandString, 'access_token':token}
		try:
		    r = requests.post(url, data = payload, timeout = 5)
		except:
			print "Error posting to particle:"



a = BusSignCloud()

while(True):
	if a.getFeed():
		a.processFeed()
		a.updateDisplay()
		a.sendToCloud()
		time.sleep(30)
	else:
		time.sleep(5)
