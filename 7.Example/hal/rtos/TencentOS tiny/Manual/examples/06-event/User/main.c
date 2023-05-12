#include "stm32f10x.h"
#include "bsp_usart.h"
#include "tos.h"


k_event_t test_event_00;

static const k_event_flag_t event_expect_00 = (k_event_flag_t)(1 << 0);
static const k_event_flag_t event_expect_01 = (k_event_flag_t)(1 << 1);
static const k_event_flag_t event_expect_02 = (k_event_flag_t)(1 << 2);

k_task_t task1;
k_task_t task2;
k_task_t task3;
k_stack_t task_stack1[1024];
k_stack_t task_stack2[1024];
k_stack_t task_stack3[1024];

void test_task1(void *Parameter)
{
    k_err_t err;
    k_event_flag_t flag_match;
    printf("task1 running\r\n");
    while(1)
    {
        err = tos_event_pend(&test_event_00, (event_expect_00 | event_expect_01 | event_expect_02),
                                &flag_match, TOS_TIME_FOREVER,
                                TOS_OPT_EVENT_PEND_ALL | TOS_OPT_EVENT_PEND_CLR);
        if(err == K_ERR_NONE)
        { 
            if(flag_match == (event_expect_00 | event_expect_01 | event_expect_02))
                printf("task1 event pend success\r\n");
        }
        
        if (err == K_ERR_PEND_DESTROY)
        {
            printf("event is destroy\r\n");
            tos_task_delay(TOS_TIME_FOREVER - 1);
        }
    }
}

void test_task2(void *Parameter)
{
    k_err_t err;
    k_event_flag_t flag_match;
    printf("task2 running\r\n");
    while(1)
    {
        err = tos_event_pend(&test_event_00, (event_expect_00 | event_expect_01 | event_expect_02),
                                &flag_match, TOS_TIME_FOREVER,
                                TOS_OPT_EVENT_PEND_ANY | TOS_OPT_EVENT_PEND_CLR);
        if(err == K_ERR_NONE)
        { 
            if(flag_match == event_expect_00)
                printf("task2 event pend event_expect_00 success\r\n");
            if(flag_match == event_expect_01)
                printf("task2 event pend event_expect_01 success\r\n");
            if(flag_match == event_expect_02)
                printf("task2 event pend event_expect_02 success\r\n");
        }
        
        if (err == K_ERR_PEND_DESTROY)
        {
            printf("event is destroy\r\n");
            tos_task_delay(TOS_TIME_FOREVER - 1);
        }
    }
}

void test_task3(void *Parameter)
{
    printf("task3 running\r\n");
    while(1)
    {
        tos_task_delay(1000);  
        tos_event_post(&test_event_00, event_expect_00);
        tos_task_delay(1000); 
        tos_event_post(&test_event_00, event_expect_01);
        tos_task_delay(1000);  
        tos_event_post(&test_event_00, event_expect_02);
        tos_task_delay(1000); 
        tos_event_post(&test_event_00, (event_expect_00|event_expect_01|event_expect_02));
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
    
    printf("create test_event_00\r\n");
    err = tos_event_create(&test_event_00, (k_event_flag_t)0);
    if(err != K_ERR_NONE)
        printf("TencentOS Create test_event_00 fail! code : %d \r\n",err);
    
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
    
    printf("create task2\r\n");
    err = tos_task_create(&task2, 
                          "task2", 
                          test_task2,
                          NULL, 
                          4, 
                          task_stack2,
                          1024,
                          20);
    if(err != K_ERR_NONE)
        printf("TencentOS Create task2 fail! code : %d \r\n",err);
    
    printf("create task3\r\n");
    err = tos_task_create(&task3, 
                          "task3", 
                          test_task3,
                          NULL, 
                          5, 
                          task_stack3,
                          1024,
                          20);
    if(err != K_ERR_NONE)
        printf("TencentOS Create task3 fail! code : %d \r\n",err);
    
    tos_knl_start(); // Start TOS Tiny

}

