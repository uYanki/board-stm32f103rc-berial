/**
  ******************************************************************************
  * File Name          : Target/tos_config.h
  * Description        : This file provides code for the configuration
  *                      of the Target/tos_config.h instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TARGET_TOS_CONFIG__H__
#define __TARGET_TOS_CONFIG__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcu_platform_M3.h"
/**
	MiddleWare name : Tencent.TencentOS-tiny.1.0.0
	MiddleWare fileName : tos_config.h
	MiddleWare version :
*/
/*---------- Whether to enable the software timer module  -----------*/
#define TOS_CFG_TIMER_EN      1

/*---------- Whether to enable the priority message mailbox module  -----------*/
#define TOS_CFG_PRIORITY_MAIL_QUEUE_EN      0

/*---------- Configure whether TencentOS tiny enables exception stack backtracking  -----------*/
#define TOS_CFG_FAULT_BACKTRACE_EN      0

/*---------- Configuring TencentOS tiny's tick frequency  -----------*/
#define TOS_CFG_CPU_TICK_PER_SECOND      1000

/*---------- Event module function macro  -----------*/
#define TOS_CFG_EVENT_EN      1

/*---------- Whether to enable the message mailbox module  -----------*/
#define TOS_CFG_MAIL_QUEUE_EN      0

/*---------- Whether to enable the Priority Message Queue module  -----------*/
#define TOS_CFG_PRIORITY_MESSAGE_QUEUE_EN      0

/*---------- set the OS clock freq,should be same of MCU core  -----------*/
#define TOS_CFG_CPU_CLOCK      80000000

/*----------   -----------*/
#define TOS_CFG_MSG_POOL_SIZE      10

/*---------- Whether to enable the mutual exclusion lock module  -----------*/
#define TOS_CFG_MUTEX_EN      1

/*---------- Configure whether to configure TIMER to function mode  -----------*/
#define TOS_CFG_TIMER_AS_PROC      1

/*---------- Configure TencentOS tiny to enable or disable the memory block management module  -----------*/
#define TOS_CFG_MMBLK_EN      1

/*---------- Whether to turn on the peripheral power management module  -----------*/
#define TOS_CFG_PWR_MGR_EN      0

/*----------   -----------*/
#define TOS_CFG_QUEUE_EN      1

/*---------- Default dynamic memory pool function macro  -----------*/
#define TOS_CFG_MMHEAP_DEFAULT_POOL_EN      1

/*---------- Configure TencentOS tiny to check if the pointer is legal  -----------*/
#define TOS_CFG_OBJECT_VERIFY_EN      1

/*----------   -----------*/
#define TOS_CFG_MSG_EN      1

/*---------- Default dynamic memory pool size  -----------*/
#define TOS_CFG_MMHEAP_DEFAULT_POOL_SIZE      0x400

/*---------- Whether to enable the message queue module  -----------*/
#define TOS_CFG_MESSAGE_QUEUE_EN      0

/*---------- Whether to enable task stack depth detection  -----------*/
#define TOS_CFG_TASK_STACK_DRAUGHT_DEPTH_DETACT_EN      0

/*---------- Configuring Tickless low power module switches  -----------*/
#define TOS_CFG_TICKLESS_EN      0

/*---------- Dynamic task creation function macro  -----------*/
#define TOS_CFG_TASK_DYNAMIC_CREATE_EN      0

/*---------- Configure the maximum number of priority levels supported by default by TencentOS tiny  -----------*/
#define TOS_CFG_TASK_PRIO_MAX      10

/*---------- Configure TencentOS tiny's kernel to enable time slice rotation  -----------*/
#define TOS_CFG_ROUND_ROBIN_EN      1

/*---------- Configure TencentOS tiny to enable the dynamic memory module or not  -----------*/
#define TOS_CFG_MMHEAP_EN      1

/*---------- Configure idle task stack size  -----------*/
#define TOS_CFG_IDLE_TASK_STK_SIZE      128

/*---------- Whether to turn on the semaphore module  -----------*/
#define TOS_CFG_SEM_EN      1

#ifdef __cplusplus
}
#endif
#endif /*__ TARGET/TOS_CONFIG__H_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
