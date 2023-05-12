void HMC5843(void)
{      
   uint8_t xh, xl, yh, yl, zh, zl;
   long xo, yo, zo;
   
   i2cSendStart();
   i2cWaitForComplete();
   i2cSendByte(0x3C);    //write to HMC
   i2cWaitForComplete();
   i2cSendByte(0x02);    //mode register
   i2cWaitForComplete();
   i2cSendByte(0x00);    //continuous measurement mode
   i2cWaitForComplete();
   i2cSendStop();
   
   //must read all six registers plus one to move the pointer back to 0x03
   i2cSendStart();
   i2cWaitForComplete();
   i2cSendByte(0x3D);          //read from HMC
   i2cWaitForComplete();
   i2cReceiveByte(TRUE);
   i2cWaitForComplete();
   xh = i2cGetReceivedByte();   //x high byte
   i2cWaitForComplete();
   //printf(" %d", xh);
   
   i2cReceiveByte(TRUE);
   i2cWaitForComplete();
   xl = i2cGetReceivedByte();   //x low byte
   i2cWaitForComplete();
   xo = xl|(xh << 8);
   printf("x=%4ld, ", xo);
   
   i2cReceiveByte(TRUE);
   i2cWaitForComplete();
   yh = i2cGetReceivedByte();   //y high byte
   i2cWaitForComplete();
   //printf(" %d ", yh);
   
   i2cReceiveByte(TRUE);
   i2cWaitForComplete();
   yl = i2cGetReceivedByte();   //y low byte
   i2cWaitForComplete();
   yo = yl|(yh << 8);
   printf("y=%4ld, ", yo);
   
   i2cReceiveByte(TRUE);
   i2cWaitForComplete();
   zh = i2cGetReceivedByte();   
   i2cWaitForComplete();      //z high byte
   //printf(" %d ", zh);
   
   i2cReceiveByte(TRUE);
   i2cWaitForComplete();
   zl = i2cGetReceivedByte();   //z low byte
   i2cWaitForComplete();
   zo = zl|(zh << 8);
   printf("z=%ld \r\n", zo);
   
   i2cSendByte(0x3D);         //must reach 0x09 to go back to 0x03
   i2cWaitForComplete();
   
   i2cSendStop();   
}