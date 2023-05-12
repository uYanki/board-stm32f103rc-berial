/*abs_addr*/
#ifndef FX2N_H
#define FX2N_H

#include "board.h"
#include <absacc.h>
#include <OS_Config.h>



void Process_switch(void);
void TX_Process(void);
void backup_data(void);
void recover_data(void);

void io_config(void);
void usart_init(void);
void process_x(void);
void y_refresh(void);
void x_refresh(void);
void init_xy(void);
void all_prog_process(void);
void TIM5_Init(void);
void NVIC_Configuration(void);

#endif
