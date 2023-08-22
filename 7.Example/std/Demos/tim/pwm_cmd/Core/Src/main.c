#include "board.h"
#include <stdlib.h>

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#include "nr_micro_shell.h"

void ecb_shell_set_compare(char argc, char* argv)
{
    char* cmd = &argv[argv[0]];

    if (argc == 3) {
        uint8_t  channel = atoi(&argv[argv[1]]);
        uint16_t value   = atoi(&argv[argv[2]]);

        switch (channel) {
            case 1: TIM_SetCompare1(TIM3, value); break;
            case 2: TIM_SetCompare2(TIM3, value); break;
            case 3: TIM_SetCompare3(TIM3, value); break;
            case 4: TIM_SetCompare4(TIM3, value); break;
            default: goto error_handler;
        }

        shell_printf("set channel %d compare value: %d\r\n", channel, value);
        return;
    }

error_handler:
    shell_printf("you should call < %s > with params: < pwm channel: 1~4 >< compare value: 0~65535 >\r\n", cmd);
}

void ecb_shell_set_clkdiv(char argc, char* argv)
{
    char* cmd = &argv[argv[0]];

    if (argc == 2) {
        uint8_t clkdiv = atoi(&argv[argv[1]]);

        switch (clkdiv) {
            case 1: TIM_SetClockDivision(TIM3, TIM_CKD_DIV1); break;
            case 2: TIM_SetClockDivision(TIM3, TIM_CKD_DIV2); break;
            case 4: TIM_SetClockDivision(TIM3, TIM_CKD_DIV4); break;
            default: goto error_handler;
        }

        shell_printf("set clock divider: %d\r\n", clkdiv);
        return;
    }

error_handler:
    shell_printf("you should call < %s > with params: < clock divider: 1,2,4 >\r\n", cmd);
}

void ecb_shell_set_reload(char argc, char* argv)
{
    char* cmd = &argv[argv[0]];

    if (argc == 2) {
        uint16_t relaod = atoi(&argv[argv[1]]);
        TIM_SetAutoreload(TIM3, relaod);  // relaod - 1
        shell_printf("set auto reload value: %d\r\n", relaod);
        return;
    }

    // error_handler:
    shell_printf("you should call < %s > with params: < auto relaod value: 0~65535 >\r\n", cmd);
}

void ecb_shell_set_prescaler(char argc, char* argv)
{
    char* cmd = &argv[argv[0]];

    if (argc == 2) {
        uint32_t prescaler = atoi(&argv[argv[1]]);
        TIM3->PSC          = prescaler;
        shell_printf("set clock prescaler: %d\r\n", prescaler);
        return;
    }

    // error_handler:
    shell_printf("you should call < %s > with params: < auto relaod value: 0~65535 >\r\n", cmd);
}

// call: ls cmd
NR_SHELL_CMD_EXPORT(set_cmpval, ecb_shell_set_compare);
NR_SHELL_CMD_EXPORT(set_clkdiv, ecb_shell_set_clkdiv);
NR_SHELL_CMD_EXPORT(set_reload, ecb_shell_set_reload);
NR_SHELL_CMD_EXPORT(set_clkpsc, ecb_shell_set_prescaler);

int main(void)
{
    board_init();

    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  // CH 1,2
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // CH 3,4
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
        TIM_OCInitTypeDef       TIM_OCInitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

        TIM_TimeBaseInitStructure.TIM_Period        = 200 - 1;
        TIM_TimeBaseInitStructure.TIM_Prescaler     = 7200 - 1;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

        TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;

        TIM_OC1Init(TIM3, &TIM_OCInitStructure);
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);

        // 预装载寄存器 CCR1
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

        TIM_Cmd(TIM3, ENABLE);
    }

    {
        shell_init();
        while (1) {
            if (SET == USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
                shell(USART_ReceiveData(USART2));  // rx char
            }
        }
    }
}
