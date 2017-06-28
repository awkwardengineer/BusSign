#include "BUS_TIMES.h"

BusTimes::BusTimes(void){
    for (int i = 0; i < 4; i++ ){
        for (int j = 0; j < 4 ; j++ ){
            //initialize array to blank digits
            //uses negative numbers as a "guard"
            timeBuf[i][j] = -1;
			dotBuf[i][j] = false;
        }
    }

}

void BusTimes::countDown(void){
	bool dot;

    for (int i = 0; i < 4; i++ ){
        for (int j = 0; j < 4 ; j++ ){

			dot = dotBuf[i][j];

            // if rowBuf is 0 or negative, it shouldn't
            // be incremented
            if (timeBuf[i][j] > 0){
                timeBuf[i][j]--;

                sendToRowBuff(i,j,timeBuf[i][j],dot);
            }
        }
    }
}

void BusTimes::setBusTime(byte row, byte nextBus,int val){
	//if no dot specified, presume it is off
	setBusTime(row, nextBus, val, false);
}

void BusTimes::setBusTime(byte row, byte nextBus,int val, bool dot){
	timeBuf[row][nextBus] = val;
	dotBuf[row][nextBus] = dot;
    sendToRowBuff(row,nextBus,val,dot);
}

void BusTimes::sendToRowBuff(byte row, byte nextBus, int val, bool dot){
	//if negative, send a negative
    if (val < 0){
        rowbuffer.setNumber(row,nextBus,-1, dot);
    }
    //if less than 60 seconds, send seconds
    else if (val < 60){
        rowbuffer.setNumber(row,nextBus,val, dot);
    }
    //otherwise, make sure it's below 99,
    else{
        if (val > (99*60)){
            val = 99;
        }
        else{
            val = floor (val / 60);
        }
        rowbuffer.setNumber(row,nextBus,val, dot);
    }
}

void BusTimes::sendToRowBuff(byte row, byte nextBus, int val){
	//if no dot, presume it is off
	sendToRowBuff(row, nextBus, val, false);
}

void BusTimes::sendAllToRowBuff(){
    for (int i = 0; i < 4; i++ ){
        for (int j = 0; j < 4 ; j++ ){

            sendToRowBuff(i,j,timeBuf[i][j],dotBuf[i][j]);

        }
    }
}

void BusTimes::updateOffset(int row){
    //rowbuffer is currently programmed on a magic
    //number 29 move updateOffset cycle
    rowbuffer.updateOffset(row);
}

void BusTimes::clearOffset(void){

    rowbuffer.clearOffset();

}
