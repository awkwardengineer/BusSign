import urllib2
from bs4 import BeautifulSoup
from  busi2c import *
import socket
import time
from LcdBlinker import *

class BusSign:
	internetLED = 1
	nextbusLED = 2	
	


	def __init__(self):
		self.myseg=SegDisplay(1,0x03)
		self.myseg2 = SegDisplay(1,0x02)
		self.Fenwick89=[]
		self.Thurston89101=[]
		self.blink = LcdBlinker()
		self.isBeating = True
	
		while True:

			self.heartbeat()
			
			if self.getFeed():
				self.processFeed()
				self.updateDisplay()	

			for i in range(0,30):
				print 30-i
				time.sleep(1)		
				#decrement the time in the register between updates
				for j in self.Fenwick89:
					j[0] -= 1	
					if j[0] <= 0:
						j[0]=0	
	
				for j in self.Thurston89101:
					j[0] -= 1	
					if j[0] <= 0:
						j[0]=0	

				self.updateDisplay()


				self.heartbeat()

	def isInternetUp(self,host="8.8.8.8",port=53,timeout=1):
		try:
			socket.setdefaulttimeout(timeout)
			socket.socket(socket.AF_INET,socket.SOCK_STREAM).connect((host,port))
			self.blink.setOffLED(self.internetLED)
			return True
		except Exception as ex:
			print ex.message
			print "No Instanets"
			self.blink.blinkLED(self.internetLED)
			return False

	def getFeed(self):
		if self.isInternetUp():
			try:
				raw_fenwick = urllib2.urlopen("http://webservices.nextbus.com/service/publicXMLFeed?command=predictions&a=mbta&stopId=2727")
				self.fenwick_soup = BeautifulSoup(raw_fenwick, "html.parser")

				raw_thurston = urllib2.urlopen("http://webservices.nextbus.com/service/publicXMLFeed?command=predictions&a=mbta&stopId=2704")
				self.thurston_soup = BeautifulSoup(raw_thurston, "html.parser")
			
					
				self.blink.setOffLED(self.nextbusLED)
				return True
			except urllib2.URLError as err:
				print type(err) 
				print "Feed is Down" 
				self.blink.blinkLED(self.nextbusLED)
				return False
			except socket.timeout as err:
				print type(err)	
				print "Instanets is down"
				self.blink.blinkLED(self.nextbusLED)
				return False
			except:
				
				self.blink.blinkLED(self.nextbusLED)
				print "Unknown Error"
		else:
			return False

	def updateDisplay(self):


		#sort on first key, which is minutes
		self.Fenwick89.sort(key=lambda x: x[0])
		
		pairOffset = 0
		for datapoint in range (0,2):
			if (datapoint + 1) > len(self.Fenwick89):
				self.myseg.turnPairOff((0+datapoint)+pairOffset)
			else:
				if self.Fenwick89[datapoint][1]=="Clarendon":
					dot = True
				else:
					dot = False
				self.myseg.writeToPair(self.Fenwick89[datapoint][0],(0+datapoint)+pairOffset,dot)

		pairOffset = 2
		for datapoint in range (0,2):

			if (datapoint + 1) > len(self.Thurston89101):
				self.myseg.turnPairOff((0+datapoint)+pairOffset)
			else:
				self.myseg.writeToPair(self.Thurston89101[datapoint][0],(0+datapoint)+pairOffset)





	def processFeed(self):

		self.Fenwick89=[]	
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

	def heartbeat(self):
		self.isBeating = not self.isBeating
	
		if self.isBeating:
			self.blink.setOnLED(0)
		else:
			self.blink.setOffLED(0)

	
sign = BusSign()
