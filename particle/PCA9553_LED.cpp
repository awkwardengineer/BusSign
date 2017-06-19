#include "PCA9553_LED.h"




byte init_PCA9553(void){

    // Initialize the I2C bus if not already enabled
    if (!Wire.isEnabled()) {
        Wire.begin();
    }

    setPWMDefaults();

    LED_Select(0, LED_OFF);
    LED_Select(1, LED_OFF);
    LED_Select(2, LED_OFF);
    LED_Select(3, LED_OFF);

}

byte LED_Select(byte val){
    /*#val is a byte, and the 4 half nibbles in each byte
	#correspond to what drives the LED setting


	#00 LED on
	#01 LED off, or high impedance output
	#10 LED blinks at PWM0
 	#11 LED blinks at PWM1 	*/

 	byte result;

 	Wire.beginTransmission(PCA9553);
 	Wire.write( LED_SELECT );
 	Wire.write( val );
    result = Wire.endTransmission();

    return result;
}

byte LED_Select(byte LED, byte mode){

    static byte settings = 0b01010101;
    byte clearbits = 0;
    byte result;

    clearbits = ~(0b11 << (2*LED));

    settings &= clearbits;
    settings |= (mode << (2*LED));


 	Wire.beginTransmission(PCA9553);
 	Wire.write( LED_SELECT );
 	Wire.write( settings );
    result = Wire.endTransmission();

    return result;

}

byte setPWMDefaults(void){

/*#define PRESCALER_0 0b001
#define PWM_0 0b010
#define PRESCALER_1 0b011
#define PWM_1 0b100*/


    byte result;


    //the prescaler period in seconds is = (1 + value) / 43
    Wire.beginTransmission(PCA9553);
    Wire.write(PRESCALER_0);
    Wire.write(42);
    result = Wire.endTransmission();

    //a non 0 result indicates a transmission error, so return that right away
    if (result) {
        return result;
    }

    Wire.beginTransmission(PCA9553);
    Wire.write(PRESCALER_1);
    Wire.write(42);
    result = Wire.endTransmission();

    if (result) {
        return result;
    }

    //the PWM dutycycle is (256 - val)/256
    Wire.beginTransmission(PCA9553);
    Wire.write(PWM_0);
    Wire.write(128);
    result = Wire.endTransmission();

    if (result) {
        return result;
    }

    Wire.beginTransmission(PCA9553);
    Wire.write(PWM_1);
    Wire.write(128);
    result = Wire.endTransmission();


    return result;
}

byte flashHeartBeat(void){
    static bool heartbeat = true;

    byte result;

    if (heartbeat){
        result = LED_Select(0, LED_ON);
        heartbeat = false;
    }
    else {
        result = LED_Select(0, LED_OFF);
        heartbeat = true;
    }

    return result;
}
