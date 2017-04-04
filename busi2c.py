import smbus
import math
import time

class SegDisplay:


	def __init__(self, i2cbus=1, addr=0x03 ):
		self.bus=smbus.SMBus(i2cbus)
		self.addr=addr

		SHUTDOWN_REG=0x0C
		SELF_ADDR_REG = 0x2D 
		BRIGHTNESS_REG = 0x0A
		FEATURE_REG = 0x0E
		SCAN_LIMIT_REG = 0x0B
		DECODE_EN_REG = 0x09
	
		#code to initialize driver follows
		
		try:	
			#wakeup
			self.bus.write_byte_data(0x00,SHUTDOWN_REG,0x01)
			time.sleep(.1)	
				
			#reset your address
			self.bus.write_byte_data(0x00,SELF_ADDR_REG,0x01)
			time.sleep(.1)
		except IOError:
				try:	
				#reset your address
					self.bus.write_byte_data(addr,SELF_ADDR_REG,0x01)
					time.sleep(.1)
				except IOError:
					print "Error with busboard: " + str(addr) 

		try:
			#set propper decoding for all digits
			self.bus.write_byte_data(addr,DECODE_EN_REG, 0xFF) 
			time.sleep(.1)
			
			#make sure all digits are blank
			#"0x0F" writes a blank display
			for digit in range (0,8):
				self.bus.write_byte_data(addr,digit+1,0x0F)
			
			#set brightness
			self.bus.write_byte_data(addr,BRIGHTNESS_REG,0x0F)

			#set scan limit to turn on all digits
			self.bus.write_byte_data(addr,SCAN_LIMIT_REG,0xFF)

		except IOError as err:
			print "i2c error"
			print err

	def writeToPair(self, data, pair=1, dot=False):
		
		if pair <= 4:
		#	pair = pair * 2
			pair = 8 - (pair * 2)
		else:
			pair = 1 * 2

		#data is in seconds	
		#if data is greater than 60, display minutes,otherwise, show seconds
		if data > 60:
			data = int(math.floor(data / 60))
			#if minutes is > 99, display 99
			if data  > 99:
				data = 99
	
		if dot:
			dot = 0b10000000
		else:
			dot = 0

		try:
			if data < 10:
				self.bus.write_byte_data(self.addr,pair-1,data | dot)
				self.bus.write_byte_data(self.addr,pair,0x0F)
			else:
				self.bus.write_byte_data(self.addr, pair-1, (data % 10) | dot)
				self.bus.write_byte_data(self.addr, pair,int(math.floor(data/10)))

		except IOError as err:
			print "i2c error"
			print err
	
	def turnPairOff(self,pair,dot=False):
		if pair <= 4:
		#	pair = pair * 2
			pair = 8 - (pair * 2)			
		else:
			pair = 1 * 2
	
		if dot:
			dot = 0b10000000
		else:
			dot = 0

		try:
			self.bus.write_byte_data(self.addr,pair-1,0x0F|dot)
			self.bus.write_byte_data(self.addr,pair,0x0F)
	
		except IOError as err:
			print "i2c error"
			print err
	
		
