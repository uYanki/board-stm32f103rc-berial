/*
 * SD模块测试程序
 *
 * 用途：SD模块测试程序
 *
 * 作者					日期				备注
 * Huafeng Lin			20010/10/03			新增
 * Huafeng Lin			20010/10/03			修改
 *
 */

#include "REG52.H"
////////////////////////****************/
unsigned char*     SDInfo1 = "SD Init Success.";
unsigned char*     SDInfo2 = "SD Init Fail.";
unsigned int xdata ReadBuffer[128];
unsigned int xdata WriteBuffer[128];
unsigned int       BlockSize;
unsigned long int  BlockNR;

// sbit sd_clk=P3^2;
// sbit sd_cse=P3^0;
// sbit sd_dai=P3^3; //Do
// sbit sd_dao=P3^1;  //DI

sbit sd_cse = P1 ^ 0;
sbit sd_dao = P1 ^ 1;  // DI
sbit sd_clk = P1 ^ 2;
sbit sd_dai = P1 ^ 3;  // Do

void Delay5us() {
    unsigned char a = 0;
    for (a = 0; a < 40; a++)
        ;
}
//********************************************
void SD_2Byte_Write(unsigned int IOData) {
    unsigned char BitCounter;

    for (BitCounter = 0; BitCounter < 16; BitCounter++) {
        sd_clk = 0;  // CLK Low

        if (IOData & 0x8000)  // If the MSB of IOData is 1, then Do=1, else Do=0.
            sd_dao = 1;       // Do High
        else
            sd_dao = 0;  // Do Low

        sd_clk = 1;  // CLK High
        Delay5us();

        IOData = IOData << 1;  // Because the MSB is transmitted firstly, shift to next lower bit.
    }
}
//********************************************
void SD_Write(unsigned int IOData) {
    unsigned char BitCounter;
    IOData = IOData << 8;

    for (BitCounter = 0; BitCounter < 8; BitCounter++) {
        sd_clk = 0;  // CLK Low

        if (IOData & 0x8000)  // If the MSB of IOData is 1, then Do=1, else Do=0.
            sd_dao = 1;       // Do High
        else
            sd_dao = 0;  // Do Low

        sd_clk = 1;  // CLK High
        Delay5us();

        IOData = IOData << 1;  // Because the MSB is transmitted firstly, shift to next lower bit.
    }
}
//********************************************
unsigned int SD_2Byte_Read() {
    unsigned int  Buffer;
    unsigned char BitCounter;
    Buffer = 0;

    for (BitCounter = 0; BitCounter < 16; BitCounter++) {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;            // CLK High
        Buffer = Buffer << 1;  // Because the MSB is transmitted firstly, shift to next lower bit.
                               // Because the LSB will be damaged, we can not put this line under next line.
        if (sd_dai)
            Buffer++;  // If SPI_Din=1 then the LSB_of_Buffer=1.
    }

    return Buffer;
}
//********************************************
unsigned int SD_Read() {
    unsigned int  Buffer;
    unsigned char BitCounter;
    Buffer = 0xffff;

    for (BitCounter = 0; BitCounter < 8; BitCounter++) {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;            // CLK High
        Buffer = Buffer << 1;  // Because the MSB is transmitted firstly, shift to next lower bit.
                               // Because the LSB will be damaged, we can not put this line under next line.
        if (sd_dai)
            Buffer++;  // If SPI_Din=1 then the LSB_of_Buffer=1.
    }

    return Buffer;
}
//********************************************
unsigned int SD_CMD_Write(unsigned int CMDIndex, unsigned long CMDArg, unsigned int ResType, unsigned int CSLowRSV)  // ResType:Response Type, send 1 for R1; send 2 for R1b; send 3 for R2.
{                                                                                                                    // There are 7 steps need to do.(marked by [1]-[7])
    unsigned int temp, Response, Response2, CRC, MaximumTimes;
    Response2    = 0;
    MaximumTimes = 10;
    CRC          = 0x0095;  // 0x0095 is only valid for CMD0
    if (CMDIndex != 0) CRC = 0x00ff;

    sd_cse = 0;  //[1] CS Low

    SD_2Byte_Write(((CMDIndex | 0x0040) << 8) + (CMDArg >> 24));  //[2] Transmit Command_Index & 1st Byte of Command_Argument.
    SD_2Byte_Write((CMDArg & 0x00ffff00) >> 8);                   //[2] 2nd & 3rd Byte of Command_Argument
    SD_2Byte_Write(((CMDArg & 0x000000ff) << 8) + CRC);           //[2] 4th Byte of Command_Argument & CRC only for CMD0

    sd_dao = 1;  //[3] Do High
                 //[3] Restore Do to High Level

    for (temp = 0; temp < 8; temp++)  //[4] Provide 8 extra clock after CMD
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    switch (ResType)  //[5] wait response
    {
        case 1:  // R1
        {
            do
                Response = SD_Read();
            while (Response == 0xffff);
            break;
        }
        case 2:  // R1b
        {
            do
                Response = SD_Read();
            while (Response == 0xffff);  // Read R1 firstly

            do
                Response2 = SD_Read() - 0xff00;
            while (Response2 != 0);  // Wait until the Busy_Signal_Token is non-zero
            break;
        }
        case 3: Response = SD_2Byte_Read(); break;  // R2
    }

    if (CSLowRSV == 0) sd_cse = 1;  //[6] CS High (if the CMD has data block response CS should be kept low)

    for (temp = 0; temp < 8; temp++)  //[7] Provide 8 extra clock after card response
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }
    return Response;
}
//********************************************
unsigned int SD_Reset_Card() {
    unsigned int temp, MaximumTimes;
    MaximumTimes = 10;

    for (temp = 0; temp < 80; temp++)  // Send 74+ Clocks
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    return SD_CMD_Write(0x0000, 0x00000000, 1, 0);  // Send CMD0
}
//********************************************
unsigned int SD_Initiate_Card()  // Polling the card after reset
{
    unsigned int temp, Response, MaximumTimes;
    MaximumTimes = 50;

    for (temp = 0; temp < MaximumTimes; temp++) {
        Response = SD_CMD_Write(0x0037, 0x00000000, 1, 0);  // Send CMD55
        Response = SD_CMD_Write(0x0029, 0x00000000, 1, 0);  // Send ACMD41
        if (Response == 0xff00)
            temp = MaximumTimes;
    }

    return Response;
}
//********************************************
unsigned int SD_Get_CardInfo()  // Read CSD register
{
    unsigned int temp, Response, MaximumTimes;
    MaximumTimes = 50;

    for (temp = 0; temp < MaximumTimes; temp++) {
        Response = SD_CMD_Write(9, 0x00000000, 1, 1);  // Send CMD9
        if (Response == 0xff00)
            temp = MaximumTimes;
    }

    for (temp = 0; temp < 8; temp++)  // Provide 8 clock to romove the first byte of data response (0x00fe)
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    for (temp = 0; temp < 8; temp++) ReadBuffer[temp] = SD_2Byte_Read();  // Get the CSD data

    for (temp = 0; temp < 16; temp++)  // Provide 16 clock to remove the last 2 bytes of data response (CRC)
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    sd_cse = 1;  // CS_High()

    for (temp = 0; temp < 8; temp++)  // Provide 8 extra clock after data response
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    BlockNR = ((ReadBuffer[3] << 2) & 0x0fff) + ((ReadBuffer[4] >> 14) & 0x0003) + 1;                          // Calcuate MULT
    BlockNR = BlockNR * (0x0002 << (((ReadBuffer[4] << 1) & 0x0007) + ((ReadBuffer[5] >> 15) & 0x0001) + 1));  // Calcuate Block_Number
    return Response;
}
//********************************************
unsigned int SD_Overall_Initiation() {
    unsigned int Response, Response_2;
    Response   = 0x0000;
    Response_2 = 0xff00;

    sd_dao = 1;  //[1] Do High
                 //[1] Do must be High when there is no transmition
    do
        Response = SD_Reset_Card();  //[2] Send CMD0
    while (Response != 0xff01);

    if (Response != 0xff01) Response_2 += 8;

    // Response=SD_CMD_Write(8,0x00000000,1,0);//Send CMD8

    Response = SD_Initiate_Card();  //[3] Send CMD55+ACMD41
    if (Response == 0xff00)
        ;
    else {
        Response_2 += 4;
        ;
    }

    do
        Response = SD_Get_CardInfo();  //[4] Read CSD
    while (Response != 0xff00);
    if (Response == 0xff01) Response_2 += 2;

    return Response_2;
    //	0000|0000||0000|0000 Response_2
    //                  |||_CSD Fail
    //                  ||__CMD55+ACMD41 Fail
    //                  |___CMD0 Fail
}
//********************************************
unsigned int SD_Get_CardID()  // Read CID register
{
    unsigned int temp, Response, MaximumTimes;
    MaximumTimes = 10;

    for (temp = 0; temp < MaximumTimes; temp++) {
        Response = SD_CMD_Write(10, 0x00000000, 1, 1);  // Send CMD9
        if (Response == 0xff00)
            temp = MaximumTimes;
    }

    for (temp = 0; temp < 8; temp++)  // Provide 8 clock to romove the first byte of data response (0x00fe)
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    for (temp = 0; temp < 8; temp++) ReadBuffer[temp] = SD_2Byte_Read();  // Get the CID data

    for (temp = 0; temp < 16; temp++)  // Provide 16 clock to remove the last 2 bytes of data response (CRC)
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    sd_cse = 1;  // CS_High()

    for (temp = 0; temp < 8; temp++)  // Provide 8 extra clock after data response
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    return Response;
}
//********************************************
unsigned int Read_Single_Block(unsigned long int BlockAddress) {
    unsigned int temp, Response, MaximumTimes;
    MaximumTimes = 10;

    if (BlockAddress > BlockNR) return 0xff20;  // whether BlockAddress out of range?

    for (temp = 0; temp < MaximumTimes; temp++) {
        Response = SD_CMD_Write(17, BlockAddress, 1, 1);  // Send CMD17
        if (Response == 0xff00)
            temp = MaximumTimes;
    }

    while (SD_Read() != 0xfffe) { ; }
    //这里为了使只有512byte的单片机能够读写SD卡，特意节省了RAM的使用量，每次读写只有两个重复的128byte
    //如果您使用的单片机拥有1K以上的RAM请将"%128"去掉
    for (temp = 0; temp < 256; temp++) ReadBuffer[temp % 128] = SD_2Byte_Read();  // Get the readed data

    for (temp = 0; temp < 16; temp++)  // Provide 16 clock to remove the last 2 bytes of data response (CRC)
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    sd_cse = 1;  // CS_High()

    for (temp = 0; temp < 8; temp++)  // Provide 8 extra clock after data response
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    return Response;
}
//********************************************
unsigned int Write_Single_Block(unsigned long int BlockAddress) {
    unsigned int temp, Response, MaximumTimes;
    MaximumTimes = 10;

    if (BlockAddress > BlockNR) return 0xff20;  // whether BlockAddress out of range?

    for (temp = 0; temp < MaximumTimes; temp++) {
        Response = SD_CMD_Write(24, BlockAddress, 1, 1);  // Send CMD24
        if (Response == 0xff00)
            temp = MaximumTimes;
    }

    for (temp = 0; temp < 8; temp++)  // Provide 8 extra clock after CMD response
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    SD_Write(0x00fe);  // Send Start Block Token
    //这里为了使只有512byte的单片机能够读写SD卡，特意节省了RAM的使用量，每次读写只有两个重复的128byte
    //如果您使用的单片机拥有1K以上的RAM请将"%128"去掉
    for (temp = 0; temp < 256; temp++) SD_2Byte_Write(WriteBuffer[temp % 128]);  // Data Block
    SD_2Byte_Write(0xffff);                                                      // Send 2 Bytes CRC

    Response = SD_Read();
    while (SD_Read() != 0xffff) { ; }

    sd_cse = 1;  // CS_High()

    for (temp = 0; temp < 8; temp++)  // Provide 8 extra clock after data response
    {
        sd_clk = 0;  // CLK Low
        Delay5us();
        sd_clk = 1;  // CLK High
        Delay5us();
    }

    return Response;
}

/* 串行通信初始化*/
void UartInit() {
    SCON = 0x50;         // uart 模式1 (8 bit), REN=1；
    TMOD = TMOD | 0x20;  // 定时器1 模式2；
                         //	TH1  = 0xF7;                // 9600 Bds at 32MHz
                         //	TL1  = 0xF7;

    TH1 = 0xFD;  // 9600 Bds at 11.059MHz
    TL1 = 0xFD;
    //	TH1  = 0xFA;                // 9600 Bds at 22.1184MHz
    //	TL1  = 0xFA;

    ES  = 1;  //允许uart中断；
    EA  = 1;  //允许CPU中断；
    TR1 = 1;  //运行定时器1：
}

void send_char_com(unsigned char ch) {
    SBUF = ch;
    while (TI == 0)
        ;
    TI = 0;
}

//向串口发送一个字符串
void send_string_com(unsigned char* str, unsigned int len) {
    unsigned char k = 0;

    do {
        send_char_com(*(str + k));
        k++;
    } while (k < len);
}

main() {
    unsigned int  Data, M_Response;
    unsigned char i;

    //初始化要写入的数据
    for (i = 0; i < 128; i++) {
        if (i < 64) {
            WriteBuffer[i] = 0x4141;
        } else {
            WriteBuffer[i] = 0x4242;
        }
    }

    UartInit();
    TI         = 0;
    M_Response = 0x0000;
    M_Response = SD_Overall_Initiation();
    M_Response = SD_CMD_Write(16, 512, 1, 0);
    Data       = SD_Get_CardID();
    Write_Single_Block(0x0000);
    Read_Single_Block(0x0000);

    for (i = 0; i < 128; i++) {
        send_char_com(ReadBuffer[i] >> 8);  //向串口发送高8位数据
        send_char_com(ReadBuffer[i]);
    }

    while (1)
        ;
}
