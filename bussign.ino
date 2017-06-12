// This #include statement was automatically added by the Particle IDE.
#include "AS1115_7SEG.h"

// This #include statement was automatically added by the Particle IDE.
#include "PCA9553_LED.h"

#include "BUS_TIMES.h"

#define SHUTDOWN_REG 0x0c
#define SELF_ADDR_REG 0x2D

int debug1;
int debug2;
int debug3 = 0;
int debug4;
int debugByte;

//RowBuffer rowbuffer;

void setup() {

    //set clock speed slow to 10khz
    Wire.setSpeed(10000);
    Wire.begin();




    Particle.variable("Debug1", debug1);
    Particle.variable("Debug2", debug2);
    Particle.variable("Debug3", debug3);
    Particle.variable("Debug4", debug4);

    Particle.variable("DebugByte", debugByte);

    Particle.function("i2creset", i2creset);
    //wakeup

    initAS1115(0x02);
    initAS1115(0x03);
    init_PCA9553();

}

void loop() {
    debug1 = flashHeartBeat();
    delay(500);
    //rowbuffer.updateOffset(-1);
    debug2 = flashHeartBeat();
    delay(500);
    //rowbuffer.updateOffset(-1);

    if (debug1 == 1){
        i2creset("astring");
    }
}

//this function can be called by the cloud
int i2creset(String astring){
    Wire.reset();
    debug3++;

    resetAS1115(0x02);
    resetAS1115(0x03);

    return 0;
}
