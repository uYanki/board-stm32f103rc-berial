

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"

// 主时钟设置选项，可选择内部DCO或外部高频晶振二者之一
// #define  INTERNAL_HF_OSC                                                     // 内部DCO时钟
#define EXTERNAL_HF_OSC  // 外部高频时钟

#ifdef INTERNAL_HF_OSC
#define INTERNAL_HF_OSC
#endif

#ifdef EXTERNAL_HF_OSC
#define EXTERNAL_HF_OSC
#endif

#define FLL_FACTOR 549  // FLL_FACTOR: DCO倍频系数

//***************************************************************************//
//                                                                           //
//                       初始化主时钟: MCLK = XT2                            //
//                                                                           //
//***************************************************************************//
#ifdef EXTERNAL_HF_OSC
void Init_CLK(void)
{
    WDTCTL = WDTPW + WDTHOLD;                                    // 关看门狗
    P5SEL |= 0x0C;                                               // 端口功能选择振荡器
    UCSCTL6 &= ~XT2OFF;                                          // 振荡器使能
    UCSCTL3 |= SELREF_2;                                         // FLLref = REFO
    UCSCTL4 |= SELA_2;                                           // ACLK=REFO,SMCLK=DCO,MCLK=DCO
    do {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);  // 清除 XT2,XT1,DCO 错误标志
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                                   // 检测振荡器错误标志
    UCSCTL6 |= XT2DRIVE0 + XT2DRIVE1;                            // XT2 驱动模式 24~32MHz
    UCSCTL4 |= SELS_5 + SELM_5;                                  // SMCLK = MCLK = XT2
}
#endif

//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                   //
//                                                                           //
//***************************************************************************//
#ifdef INTERNAL_HF_OSC
void Init_CLK(void)
{
    WDTCTL = WDTPW + WDTHOLD;        // 关看门狗
    P7SEL |= 0x03;                   // 端口选择外部低频晶振XT1
    UCSCTL6 &= ~XT1OFF;              // 使能外部晶振
    UCSCTL6 |= XCAP_3;               // 设置内部负载电容
    UCSCTL3 |= SELREF_2;             // DCOref = REFO
    UCSCTL4 |= SELA_0;               // ACLK   = XT1
    __bis_SR_register(SCG0);         // 关闭FLL控制回路
    UCSCTL0 = 0x0000;                // 设置DCOx, MODx
    UCSCTL1 = DCORSEL_7;             // 设置DCO振荡范围
    UCSCTL2 = FLLD__1 + FLL_FACTOR;  // Fdco = ( FLL_FACTOR + 1)×FLLRef = (549 + 1) * 32768 = 18.0224MHz
    __bic_SR_register(SCG0);         // 打开FLL控制回路
    __delay_cycles(1024000);
    do {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);  // 清除 XT2,XT1,DCO 错误标志
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                                   // 检测振荡器错误标志
}
#endif

void Uart_Init(void)
{
    P10SEL |= BIT4 + BIT5;
    P10DIR |= BIT4;
    UCA3CTL1 = UCSWRST;            // 状态机复位
    UCA3CTL1 |= UCSSEL_1;          // CLK = ACLK
    UCA3BR0  = 0x03;               // 32kHz/9600=3.41
    UCA3BR1  = 0x00;
    UCA3MCTL = UCBRS_3 + UCBRF_0;  // UCBRSx=3, UCBRFx=0
    UCA3CTL1 &= ~UCSWRST;          // 启动状态机
    UCA3IE |= UCRXIE;              // 允许接收中断
}

//***************************************************************************//
//                                                                           //
//                 主程序: 设置时钟并输出至P11.0~2                           //
//                                                                           //
//***************************************************************************//

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;  // 关闭看门狗
    Init_CLK();
    Uart_Init();

    P1DS  = BIT0 + BIT6;  // 选择驱动强度
    P1SEL = BIT0 + BIT6;  // 选择引脚功能，ACK、MCK、SMCK
    P1DIR = BIT0 + BIT6;  // 设置引脚IO方向为输出

    //_EINT();
    __bis_SR_register(LPM0_bits + GIE);  // Enter LPM0, interrupts enabled
    __no_operation();                    // For debugger

    while (1)
        ;
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector = USCI_A3_VECTOR

__interrupt void USCI_A3_ISR(void)
{
    switch (__even_in_range(UCA3IV, 4)) {
        case 0: break;              // Vector 0 - no interrupt
        case 2:                     // Vector 2 - RXIFG
            while (!(UCA3IFG & UCTXIFG))
                ;                   // USCI_A0 TX buffer ready?
            UCA3TXBUF = UCA3RXBUF;  // TX -> RXed character
            break;
        case 4: break;              // Vector 4 - TXIFG
        default: break;
    }
}

// end of file