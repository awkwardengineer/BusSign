import smbus
import math
import time

class LcdBlinker:

	AUTO_INC_FLAG = 0b00010000
	INPUT = 0b000
	PRESCALER_0 = 0b001 
	PWM_0 = 0b010
	PRESCALER_1 = 0b011
	PWM_1 = 0b100
	LED_SELECT = 0b101
 


	def __init__(self, i2cbus=1, addr=0x62 ):

		self.bus=smbus.SMBus(i2cbus)
		self.addr=addr
		self.LEDregister = 0b01010101	#all off

		self.setLED_Select()
		self.setPrescaler0()
		self.setPrescaler1()
		self.setPWM0()
		self.setPWM1()
		self.setLED_Select()
		
	def setPrescaler0(self, val=43):
		#the prescaler period in seconds is = (1 + value) / 43
			
		self.bus.write_byte_data(self.addr,self.PRESCALER_0,val)
	
	def setPrescaler1(self, val=43):
		#the prescaler period in seconds is = (1 + value) / 43
			
		self.bus.write_byte_data(self.addr,self.PRESCALER_1,val)

	def setPWM0(self, val=128):
		#the PWM dutycycle is (256 - val)/256
			
		self.bus.write_byte_data(self.addr,self.PWM_0,val)

	def setPWM1(self, val=128):
		#the prescaler period in seconds is = (1 + value) / 43
		
		self.bus.write_byte_data(self.addr,self.PWM_1,val)

	def setLED_Select(self, val=None):
		#val is a byte, and the 4 half nibbles in each byte
		#correspond to what drives the LED setting
		

		#00 LED on 
		#01 LED off, or high impedance output
		#10 LED blinks at PWM0
 		#11 LED blinks at PWM1 	

		if val == None:
			val = self.LEDregister

		self.bus.write_byte_data(self.addr,self.LED_SELECT,val)

	def setOnLED(self, LED=0):

		#takes the complement of the shifted bits, so should be
		#0b11111100, 11110011, 11001111, 00111111
		setting = ~ (0b11 << (LED*2))
		
		self.LEDregister = self.LEDregister & setting
		self.setLED_Select()

	def setOffLED(self, LED=0):
		offbits = ~ (0b10 << (LED*2))
		onbits = 0b01 << (LED*2) 			
		
		self.LEDregister = self.LEDregister & offbits
		self.LEDregister = self.LEDregister | onbits

		self.setLED_Select()

	def blinkLED(self,LED=0,source=0):

		if source == 0:
			offbits = ~ (0b10 << (LED*2))
			onbits = 0b10 << (LED*2)			
		else:
			offbits = ~ (0b11 << (LED*2))
			onbits = 0b11 << (LED*2) 			

		self.LEDregister = self.LEDregister & offbits
		self.LEDregister = self.LEDregister | onbits

		
		self.setLED_Select()
		self.setLED_Select()

