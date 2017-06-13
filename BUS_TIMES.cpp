#include "BUS_TIMES.h"

BusTimes::BusTimes(void){
    for (int i = 0; i < 4; i++ ){
        for (int j = 0; j < 4 ; j++ ){
            //initialize array to blank digits
            //uses negative numbers as a "guard"
            timeBuf[i][j] = -1;
        }
    }

}

void BusTimes::countDown(void){
    for (int i = 0; i < 4; i++ ){
        for (int j = 0; j < 4 ; j++ ){

            // if rowBuf is 0 or negative, it shouldn't
            // be incremented
            if (timeBuf[i][j] > 0){
                timeBuf[i][j]--;
                sendToRowBuff(i,j,timeBuf[i][j]);
            }
        }
    }
}

void BusTimes::setBusTime(byte row, byte nextBus,int val){
    timeBuf[row][nextBus] = val;
    sendToRowBuff(row,nextBus,val);
}

void BusTimes::sendToRowBuff(byte row, byte nextBus, int val){
    //if negative, send a negative
    if (val < 0){
        rowbuffer.setNumber(row,nextBus,-1);
    }
    //if less than 60 seconds, send seconds
    else if (val < 60){
        rowbuffer.setNumber(row,nextBus,val);
    }
    //otherwise, make sure it's below 99,
    else{
        if (val > (99*60)){
            val = 99;
        }
        else{
            val = floor (val / 60);
        }
        rowbuffer.setNumber(row,nextBus,val);
    }

}

void BusTimes::sendAllToRowBuff(){
    for (int i = 0; i < 4; i++ ){
        for (int j = 0; j < 4 ; j++ ){

            sendToRowBuff(i,j,timeBuf[i][j]);

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
