// This #include statement was automatically added by the Particle IDE.
#include "AS1115_7SEG.h"

// This #include statement was automatically added by the Particle IDE.
#include "PCA9553_LED.h"

#include "BUS_TIMES.h"

#define SHUTDOWN_REG 0x0c
#define SELF_ADDR_REG 0x2D

int debug1;
String debug2;
int debug3 = 0;
int debug4;
int debugByte;

//RowBuffer rowbuffer;
BusTimes bustimes;

void setup() {

    //set clock speed slow to 10khz
    //Wire.setSpeed(10000);
    Wire.begin();




    Particle.variable("Debug1", debug1);
    Particle.variable("Debug2", debug2);
    Particle.variable("Debug3", debug3);
    Particle.variable("Debug4", debug4);

    Particle.variable("DebugByte", debugByte);

    Particle.function("i2creset", i2creset);
    Particle.function("setBusTimes", parseMessage);
    //wakeup

    initAS1115(0x02);
    initAS1115(0x03);
    init_PCA9553();

    bustimes.setBusTime(0, 0, 10);
    bustimes.setBusTime(0, 1, 20);
    bustimes.setBusTime(0, 2, 30);
    bustimes.setBusTime(0, 3, 40);

    bustimes.setBusTime(1, 0, 500);
    bustimes.setBusTime(1, 1, 600);
    bustimes.setBusTime(1, 2, 700);
    bustimes.setBusTime(1, 3, 800);

    bustimes.setBusTime(2, 0, 90);
    bustimes.setBusTime(3, 0, 9);

}

void loop() {
    for(int i=0; i<3;i++){
        debug1 = flashHeartBeat();
        delay(500);

        debug1 = flashHeartBeat();
        delay(500);

        if (debug1 == 1){
            i2creset("astring");
        }

        bustimes.countDown();
    }

    LED_Select(1, LED_ON);


    for(int i=0; i<8;i++){

        debug1 = flashHeartBeat();
        delay(500);

        //bustimes.updateOffset(-1);

        debug1 = flashHeartBeat();
        delay(500);
        //bustimes.updateOffset(-1);

        if (debug1 == 1){
            i2creset("astring");
        }

        bustimes.countDown();

    }

    LED_Select(1, LED_OFF);

    bustimes.clearOffset();

}

//this function can be called by the cloud
int i2creset(String astring){
    Wire.reset();
    debug3++;

    resetAS1115(0x02);
    resetAS1115(0x03);

    return 0;
}

int parseMessage(String msg){
//message format is a structured array

//000s000s000s000m000m000m000s000S000M
//16 sets of 4 bytes. the first 3 bytes in each set are a time, the 4th byte
//is whether that time is in minutes or seconds.
//the 16 sets correspond to 4 rows of 4 times

//.toInt()
//.substring(from, to) from is inclusive, to is exclusive
//.char at

String submsg;
char type;
int nextbustime;

    for(int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            type = msg.charAt( 16*i + 4*j + 3);
            submsg = msg.substring(16*i + 4*j, 16*i + 4*j + 3);

            if (i == 4 && j ==4){
                //unfortunately, the buffer is 63 characters, not 64
                // so we have to skip the very last iteration in the loop
                nextbustime = submsg.toInt() * 60;
            }
            else{
                switch (type){
                    case 's':
                    //time in seconds
                        nextbustime = submsg.toInt();
                        break;

                    case 'm':
                        nextbustime = submsg.toInt() * 60;
                        break;

                    default:
                        nextbustime = -1 ;
                        break;

                }
            }

            bustimes.setBusTime(i,j,nextbustime);
        }
    }

    /*type = msg.charAt( 3);
    submsg = msg.substring(0, 3);
    debug2 = type;
    nextbustime = submsg.toInt();
    bustimes.setBusTime(0,0,nextbustime);
*/
    return 0;
}
