

#include <intrins.h>
#include <reg51.h>

#define uchar unsigned char
#define uint unsigned int
#define delayNOP() \
    ;              \
    {              \
        _nop_();   \
        _nop_();   \
        _nop_();   \
        _nop_();   \
    };

void delay(uchar x);  // x*0.14MS
void delay1(int ms);
void beep();

sbit IRIN  = P3 ^ 2;  //红外接收器数据线
sbit BEEP  = P1 ^ 5;  //蜂鸣器驱动线
sbit RELAY = P1 ^ 4;  //继电器驱动线

uchar IRCOM[7];

// LCD1602
sbit LCD_RS = P2 ^ 6;
sbit LCD_RW = P2 ^ 5;
sbit LCD_EN = P2 ^ 7;

uchar code cdis1[] = {" Red Control "};
uchar code cdis2[] = {"  IR-CODE: --H  "};

/*******************************************************************/
/*                                                                 */
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
/*                                                                 */
/*******************************************************************/

bit lcd_busy() {
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(P0 & 0x80);
    LCD_EN = 0;
    return (result);
}

/*******************************************************************/
/*                                                                 */
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
/*                                                                 */
/*******************************************************************/

void lcd_wcmd(uchar cmd)

{
    while (lcd_busy())
        ;
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_();
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;
}

/*******************************************************************/
/*                                                                 */
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
/*                                                                 */
/*******************************************************************/

void lcd_wdat(uchar dat) {
    while (lcd_busy())
        ;
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    P0     = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;
}

/*******************************************************************/
/*                                                                 */
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/

void lcd_init() {
    delay1(15);
    lcd_wcmd(0x38);  // 16*2显示，5*7点阵，8位数据
    delay1(5);
    lcd_wcmd(0x38);
    delay1(5);
    lcd_wcmd(0x38);
    delay1(5);

    lcd_wcmd(0x0c);  //显示开，关光标
    delay1(5);
    lcd_wcmd(0x06);  //移动光标
    delay1(5);
    lcd_wcmd(0x01);  //清除LCD的显示内容
    delay1(5);
}

/*******************************************************************/
/*                                                                 */
/*  设定显示位置                                                   */
/*                                                                 */
/*******************************************************************/

void lcd_pos(uchar pos) {
    lcd_wcmd(pos | 0x80);  //数据指针=80+地址变量
}

/*******************************************************************/
void main() {
    uchar m;

    IE   = 0x81;  //允许总中断中断,使能 INT0 外部中断
    TCON = 0x01;  //触发方式为脉冲负边沿触发

    IRIN  = 1;  // I/O口初始化
    BEEP  = 1;
    RELAY = 1;

    delay1(10);  //延时
    lcd_init();  //初始化LCD

    lcd_pos(0);  //设置显示位置为第一行的第1个字符
    m = 0;
    while (cdis1[m] != '\0') {  //显示字符
        lcd_wdat(cdis1[m]);
        m++;
    }

    lcd_pos(0x40);  //设置显示位置为第二行第1个字符
    m = 0;
    while (cdis2[m] != '\0') {
        lcd_wdat(cdis2[m]);  //显示字符
        m++;
    }

    while (1)
        ;

}  // end main
/**********************************************************/
void IR_IN() interrupt 0 using 0 {
    unsigned char j, k, N = 0;
    EX0 = 0;
    delay(15);
    if (IRIN == 1) {
        EX0 = 1;
        return;
    }
    //确认IR信号出现
    while (!IRIN)  //等IR变为高电平，跳过9ms的前导低电平信号。
    { delay(1); }

    for (j = 0; j < 4; j++)  //收集四组数据
    {
        for (k = 0; k < 8; k++)  //每组数据有8位
        {
            while (IRIN)  //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
            { delay(1); }
            while (!IRIN)  //等 IR 变为高电平
            { delay(1); }
            while (IRIN)  //计算IR高电平时长
            {
                delay(1);
                N++;
                if (N >= 30) {
                    EX0 = 1;
                    return;
                }                                        // 0.14ms计数过长自动离开。
            }                                            //高电平计数完毕
            IRCOM[j] = IRCOM[j] >> 1;                    //数据最高位补“0”
            if (N >= 8) { IRCOM[j] = IRCOM[j] | 0x80; }  //数据最高位补“1”
            N = 0;
        }  // end for k
    }      // end for j

    if (IRCOM[2] != ~IRCOM[3]) {
        EX0 = 1;
        return;
    }

    IRCOM[5] = IRCOM[2] & 0x0F;  //取键码的低四位
    IRCOM[6] = IRCOM[2] >> 4;    //右移4次，高四位变为低四位

    if (IRCOM[5] > 9) {
        IRCOM[5] = IRCOM[5] + 0x37;
    } else
        IRCOM[5] = IRCOM[5] + 0x30;

    if (IRCOM[6] > 9) {
        IRCOM[6] = IRCOM[6] + 0x37;
    } else
        IRCOM[6] = IRCOM[6] + 0x30;

    lcd_pos(0x4b);
    lcd_wdat(IRCOM[6]);  //第一位数显示
    lcd_pos(0x4c);
    lcd_wdat(IRCOM[5]);  //第二位数显示

    beep();
    EX0 = 1;
}

/**********************************************************/
void beep() {
    unsigned char i;
    for (i = 0; i < 100; i++) {
        delay(4);
        BEEP = !BEEP;  // BEEP取反
    }
    BEEP = 1;  //关闭蜂鸣器
}
/**********************************************************/
void delay(unsigned char x)  // x*0.14MS
{
    unsigned char i;
    while (x--) {
        for (i = 0; i < 13; i++) {}
    }
}

/**********************************************************/
void delay1(int ms) {
    unsigned char y;
    while (ms--) {
        for (y = 0; y < 250; y++) {
            _nop_();
            _nop_();
            _nop_();
            _nop_();
        }
    }
}
