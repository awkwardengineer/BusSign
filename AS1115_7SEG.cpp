#include "AS1115_7SEG.h"


byte initAS1115(byte addr){

    //sends a wakeup command
    Wire.beginTransmission(0x00);
    Wire.write(SHUTDOWN_REG);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(100);

    //sends an i2c address reset
    Wire.beginTransmission(0x00);
    Wire.write(SELF_ADDR_REG);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(100);

    return resetAS1115(addr);

}

byte resetAS1115(byte addr){
    //sends a wakeup command
    Wire.beginTransmission(addr);
    Wire.write(SHUTDOWN_REG);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(100);

    //turn off all the digits
    for (int i = 0; i < 8 ; i++){
        //note, the first digit starts at register 0x01
        //0x0F is the code for a blank digit

        Wire.beginTransmission(addr);
        Wire.write( i + 1);
        Wire.write(0x0F);
        Wire.endTransmission();

    }

    //set propper decoding for all digits
    Wire.beginTransmission(addr);
    Wire.write(DECODE_EN_REG);
    Wire.write(0xFF);
    Wire.endTransmission();
    //delay(100);



    //set brightness
    Wire.beginTransmission(addr);
    Wire.write(BRIGHTNESS_REG);
    Wire.write(0x0F);
    Wire.endTransmission();
    //delay(100);

	//set scan limit to turn on all digits
    Wire.beginTransmission(addr);
    Wire.write(SCAN_LIMIT_REG);
    Wire.write(0xFF);
    byte result = Wire.endTransmission();
    //delay(100);

    return result;


}

byte updateDigit(byte position, byte digit){

    static byte digits[16];
    byte addr;

    //this code sends the digit to the correct display
    if (position < 8){
        addr = 0x03;
    }
    else{
        addr = 0x02;
        position = position - 8;
    }

    //this code reverses the order of the digits so it appears
    //in the correct sequence. (the board is kinda sorta wired backwards)
    if (position < 4){
        position = 3 - position;
    }
    else{
        position = 11 - position;
    }

    Wire.beginTransmission(addr);
    Wire.write( position + 1);
    Wire.write(digit);
    return Wire.endTransmission();

}



RowBuffer::RowBuffer(void){
    for (int i = 0; i < 4; i++ ){
        for (int j = 0; j < 12 ; j++ ){
            //initialize array to blank digits
            rowBuf[i][j] = 0x0F;
        }
    }
}

void RowBuffer::setNumber(byte row, byte pos, int val){
    byte lowerdigit;
    byte upperdigit;

    if (val>99){
        val = 99;
    }

    if (val < 0){
        //negative number clears the digit
        lowerdigit = 0x0F;
        upperdigit = 0x0F;

    }
    else{
        lowerdigit = (byte) val % 10;

        upperdigit = (byte) floor ( val / 10) ;
        if (upperdigit == 0){
            upperdigit = 0x0F;
        }
    }

    rowBuf[row][pos*2] = upperdigit;
    rowBuf[row][pos*2 + 1] = lowerdigit;

    updateDisplay();
}

void RowBuffer::updateDisplay(void){

    byte pos[4];
    byte digVal[4];
    int shift;
    int midshift;

    for (int i = 0; i < 4; i++ ){


            pos[0] = (i * 4);
            pos[1] = pos[0] + 1;
            pos[2] = pos[0] + 2;
            pos[3] = pos[0] + 3;

            shift = offset[i];

            midshift = shift % 7;
            shift = floor(shift / 7);


            switch (midshift){
/*                case 0:
                    digVal[0] =
                    digVal[1] =
                    digVal[2] =
                    digVal[3] = rowBuf[i][shift];
                    break;
                case 0:
                    digVal[0] =
                    digVal[1] =
                    digVal[2] = rowBuf[i][shift];
                    digVal[3] = rowBuf[i][shift+1];
                    break;
                case 0:
                    digVal[0] =
                    digVal[1] = rowBuf[i][shift];
                    digVal[2] = rowBuf[i][shift+1];
                    digVal[3] =
                    break;
                case 0:
                    digVal[0] =
                    digVal[1] = rowBuf[i][shift];
                    digVal[2] = rowBuf[i][shift+1];
                    digVal[3] =
                    break;
*/

                case 0:
                    digVal[0] = rowBuf[i][shift];
                    digVal[1] = rowBuf[i][shift+1];
                    digVal[2] = rowBuf[i][shift+2];
                    digVal[3] = rowBuf[i][shift+3];
                    break;
                case 1:
                    digVal[0] = rowBuf[i][shift+1];
                    digVal[1] = 0x0F;
                    digVal[2] = rowBuf[i][shift+2];
                    digVal[3] = rowBuf[i][shift+3];
                    break;
                case 2:
                    digVal[0] = 0x0F;
                    digVal[1] = 0x0F;
                    digVal[2] = rowBuf[i][shift+2];
                    digVal[3] = rowBuf[i][shift+3];
                    break;
                case 3:
                    digVal[0] = 0x0F;
                    digVal[1] = rowBuf[i][shift+2];
                    digVal[2] = rowBuf[i][shift+3];
                    digVal[3] = 0x0F;
                    break;
                case 4:
                    digVal[0] = rowBuf[i][shift+2];
                    digVal[1] = rowBuf[i][shift+3];
                    digVal[2] = 0x0F;
                    digVal[3] = 0x0F;
                    break;
                case 5:
                    digVal[0] = rowBuf[i][shift+2];
                    digVal[1] = rowBuf[i][shift+3];
                    digVal[2] = 0x0F;
                    digVal[3] = rowBuf[i][shift+4];
                    break;
                case 6:
                    digVal[0] = rowBuf[i][shift+2];
                    digVal[1] = rowBuf[i][shift+3];
                    digVal[2] = rowBuf[i][shift+4];
                    digVal[3] = rowBuf[i][shift+5];
                    offset[i] += 7;
                    break;

            }


            updateDigit(pos[0], digVal[0]);
            updateDigit(pos[1], digVal[1]);
            updateDigit(pos[2], digVal[2]);
            updateDigit(pos[3], digVal[3]);

    }
}

byte RowBuffer::updateOffset(int row){
    if (row < 0){
        //negative offset means shift all rows
        offset[0]++;
        offset[1]++;
        offset[2]++;
        offset[3]++;
    }
    else{
        offset[row]++;
    }

    for (int i=0; i<4;i++){

        if (offset[i] > 28){
            offset[i] = 0;
        }
    }

    updateDisplay();

    return offset[0];
}
