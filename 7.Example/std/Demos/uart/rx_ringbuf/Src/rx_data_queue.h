#ifndef __ESP_DATA_QUEUE_H_
#define __ESP_DATA_QUEUE_H_

#include "stm32f10x.h"

#include <string.h>
#include <stdio.h>

//缓冲队列的个数需要为2的幂
#define QUEUE_NODE_NUM        (2)            //缓冲队列的大小（有多少个缓冲区）
#define QUEUE_NODE_DATA_LEN   (2*1024 )       //单个接收缓冲区大小

//队列的主体数据类型接口
#define QUEUE_DATA_TYPE  				ESP_USART_FRAME
//队列的调试输出接口
#define DATA_QUEUE_LOG  				QUEUE_DEBUG
#define DATA_QUEUE_LOG_ARRAY 	QUEUE_DEBUG_ARRAY


//数据主体
typedef struct 
{
	char  *head; 	//缓冲区头指针	
	uint16_t len; //接收到的数据长度

}ESP_USART_FRAME;


//队列结构
typedef struct {
	int         size;  /* 缓冲区大小          */
	int         read; /* 读指针              */
	int         write;   /* 写指针  */
	int read_using;	/*正在读取的缓冲区指针*/
	int write_using;		/*正在写入的缓冲区指针*/
	QUEUE_DATA_TYPE    *elems[QUEUE_NODE_NUM];  /* 缓冲区地址                   */
} QueueBuffer;


extern QueueBuffer rx_queue;




/*信息输出*/
#define QUEUE_DEBUG_ON         1
#define QUEUE_DEBUG_ARRAY_ON		1

#define QUEUE_INFO(fmt,arg...)           printf("<<-QUEUE-INFO->> "fmt"\n",##arg)
#define QUEUE_ERROR(fmt,arg...)          printf("<<-QUEUE-ERROR->> "fmt"\n",##arg)
#define QUEUE_DEBUG(fmt,arg...)          do{\
                                          if(QUEUE_DEBUG_ON)\
                                          printf("<<-QUEUE-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define QUEUE_DEBUG_ARRAY(array, num)    do{\
																									 int32_t i;\
																									 uint8_t* a = array;\
																									 if(QUEUE_DEBUG_ARRAY_ON)\
																									 {\
																											printf("\n<<-QUEUE-DEBUG-ARRAY->>\n");\
																											for (i = 0; i < (num); i++)\
																											{\
																													printf("%02x   ", (a)[i]);\
																													if ((i + 1 ) %10 == 0)\
																													{\
																															printf("\n");\
																													}\
																											}\
																											printf("\n");\
																									}\
																								 }while(0)	

//输出队列的状态信息
#define cbPrint(cb)		    DATA_QUEUE_LOG("size=0x%x, read=%d, write=%d\n", cb.size, cb.read, cb.write);\
	  DATA_QUEUE_LOG("size=0x%x, read_using=%d, write_using=%d\n", cb.size, cb.read_using, cb.write_using);


QUEUE_DATA_TYPE* cbWrite(QueueBuffer *cb);
QUEUE_DATA_TYPE* cbRead(QueueBuffer *cb);
void cbReadFinish(QueueBuffer *cb);
void cbWriteFinish(QueueBuffer *cb);
//void cbPrint(QueueBuffer *cb);
QUEUE_DATA_TYPE* cbWriteUsing(QueueBuffer *cb) ;
int cbIsFull(QueueBuffer *cb) ; 
int cbIsEmpty(QueueBuffer *cb) ;

void rx_queue_init(void);
void pull_data_from_queue(void);
void push_data_to_queue(char *src_dat,uint16_t src_len);


#endif



