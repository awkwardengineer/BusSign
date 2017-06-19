#ifndef PCA9553_LED_H
#define PCA9553_LED_H
#include "application.h"


//
#define PCA9553 0x62
#define AUTO_INC_FLAG 0b00010000
#define INPUT  0b000
#define PRESCALER_0 0b001
#define PWM_0 0b010
#define PRESCALER_1 0b011
#define PWM_1 0b100
#define LED_SELECT 0b101


#define LED_ON  0b00
#define LED_OFF  0b01
#define LED_BLINK_PWM0  0b10
#define LED_BLINK_PWM1  0b11

	    /*#val is a byte, and the 4 half nibbles in each byte
		#correspond to what drives the LED setting


		#00 LED on
		#01 LED off, or high impedance output
		#10 LED blinks at PWM0
 		#11 LED blinks at PWM1 	*/



byte init_PCA9553(void);
byte LED_Select(byte val);
byte LED_Select(byte LED, byte mode);
byte setPWMDefaults(void);
byte flashHeartBeat(void);


#endif
