#include "stm32f10x.h"
#include "bsp_usart.h"
#include "tos.h"


k_msg_queue_t test_msg_queue_00;

k_task_t task1;
k_task_t task2;
k_stack_t task_stack1[1024];
k_stack_t task_stack2[1024];


void test_task1(void *Parameter)
{
    k_err_t err;
    int i = 0;
    int msg_received;
    size_t msg_size = 0;
    
    while(1)
    {
        printf("queue pend\r\n");
        for (i = 0; i < 3; ++i) 
        {
            err = tos_msg_queue_get(&test_msg_queue_00, (void **)&msg_received, &msg_size);
            if (err == K_ERR_NONE)
                printf("msg queue get is %d \r\n",msg_received);
            
            if (err == K_ERR_PEND_DESTROY)
            {
                printf("queue is destroy\r\n");
                tos_task_delay(TOS_TIME_FOREVER - 1);
            }
        }
        tos_task_delay(1000); 
    }
}

void test_task2(void *Parameter)
{
    k_err_t err;
    
    int i = 0;
    uint32_t msgs[3] = { 1, 2, 3 };

    printf("task2 running\r\n");

    while(1)
    { 
        for (i = 0; i < 3; ++i) 
        {
            err = tos_msg_queue_put(&test_msg_queue_00, (void *)(msgs[i]), sizeof(uint32_t), TOS_OPT_MSG_PUT_FIFO);
            if (err != K_ERR_NONE)
                printf("msg queue put fail! code : %d \r\n",err);
        }
        
        tos_task_delay(1000);       
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
    
    /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
    USART_Config();

    printf("Welcome to TencentOS tiny\r\n");

    tos_knl_init(); // TOS Tiny kernel initialize
    
    tos_robin_config(TOS_ROBIN_STATE_ENABLED, (k_timeslice_t)500u);
    
    printf("create test_queue_00\r\n");
    err = tos_msg_queue_create(&test_msg_queue_00);
    if(err != K_ERR_NONE)
        printf("TencentOS Create test_msg_queue_00 fail! code : %d \r\n",err);
    
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
    
    tos_knl_start(); // Start TOS Tiny

}

