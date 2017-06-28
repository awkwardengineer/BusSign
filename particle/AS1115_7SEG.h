#ifndef AS1115_7SEG_H
#define AS1115_7SEG_H

#include "application.h"
#include "math.h"


#define SHUTDOWN_REG 0x0C
#define	SELF_ADDR_REG 0x2D
#define	BRIGHTNESS_REG  0x0A
#define	FEATURE_REG  0x0E
#define	SCAN_LIMIT_REG  0x0B
#define DECODE_EN_REG 0x09


byte initAS1115(byte addr);
byte updateDigit(byte position, byte digit);
byte resetAS1115(byte addr);

class RowBuffer {
    private:
        byte rowBuf[4][12];
        int offset[4];
        void updateDisplay(void);

    public:
        void setNumber(byte row, byte pos, int val);
		void setNumber(byte row, byte pos, int val, bool dot);
        void clearRows();
        void clearOffset();
        byte updateOffset(int row);
        RowBuffer(void);
};

#endif
