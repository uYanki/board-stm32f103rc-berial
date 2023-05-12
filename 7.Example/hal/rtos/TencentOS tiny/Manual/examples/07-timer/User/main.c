#include "stm32f10x.h"
#include "bsp_usart.h"
#include "tos.h"

k_timer_t test_timer_00;
k_timer_t test_timer_01;


k_task_t task1;
k_stack_t task_stack1[1024];

static void test_timer_one_call_back(void *arg)
{
    printf("timer one callback\r\n");
}

static void test_timer_periodic_call_back(void *arg)
{
    printf("timer periodic callback\r\n");
}

void test_task1(void *Parameter)
{
    while(1)
    {
        printf("task1 running\r\n");
        tos_task_delay(2000);
    }
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    k_err_t err;
    
    USART_Config();

    printf("Welcome to TencentOS tiny\r\n");

    tos_knl_init(); // TOS Tiny kernel initialize
    
    tos_robin_config(TOS_ROBIN_STATE_ENABLED, (k_timeslice_t)500u);

    err = tos_timer_create(&test_timer_00, (k_tick_t)5000u, (k_tick_t)0u,
                        test_timer_one_call_back, K_NULL,
                        TOS_OPT_TIMER_ONESHOT);
    if(err != K_ERR_NONE)
        printf("TencentOS Create test_timer_00 fail! code : %d \r\n",err);
    
    err = tos_timer_start(&test_timer_00);
    
    if(err != K_ERR_NONE)
        printf("TencentOS Create start test_timer_00 fail! code : %d \r\n",err);
    
    
    err = tos_timer_create(&test_timer_01, (k_tick_t)0, (k_tick_t)1000,
                    test_timer_periodic_call_back, K_NULL,
                    TOS_OPT_TIMER_PERIODIC);  
    if(err != K_ERR_NONE)
        printf("TencentOS Create test_timer_01 fail! code : %d \r\n",err);
    
    err = tos_timer_start(&test_timer_01);
    
    if(err != K_ERR_NONE)
        printf("TencentOS Create start test_timer_01 fail! code : %d \r\n",err);
    
    printf("create task1\r\n");
    err = tos_task_create(&task1, 
                          "task1", 
                          test_task1,
                          NULL, 
                          3, 
                          task_stack1,
                          1024,
                          20);
    if(err != K_ERR_NONE)
        printf("TencentOS Create task1 fail! code : %d \r\n",err);
 
    tos_knl_start(); // Start TOS Tiny

}

