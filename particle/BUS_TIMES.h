#ifndef BUS_TIMES_H
#define BUS_TIMES_H

#include "AS1115_7SEG.h"

//holds an array of bus times in seconds,



class BusTimes{
    private:
        int timeBuf[4][4];
		bool dotBuf[4][4];
        void sendToRowBuff(byte row, byte nextBus, int val);
		void sendToRowBuff(byte row, byte nextBus, int val, bool dot);
        RowBuffer rowbuffer;

    public:
        void countDown(void);
        void sendAllToRowBuff();
        void setBusTime(byte row, byte nextBus,int val);
		void setBusTime(byte row, byte nextBus,int val, bool dot);
        void updateOffset(int row);
        void clearOffset(void);
        BusTimes(void);
};


#endif
