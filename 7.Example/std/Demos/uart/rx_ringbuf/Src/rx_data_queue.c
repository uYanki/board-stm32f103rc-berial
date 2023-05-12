/**
  ******************************************************************************
  * @file    rx_data_queue.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   环形缓冲区，适用于接收外部数据时用作缓冲
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 IOT STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./usart/rx_data_queue.h"



//实例化节点数据类型
QUEUE_DATA_TYPE  node_data[QUEUE_NODE_NUM]; 
//实例化队列类型
QueueBuffer rx_queue;

//队列缓冲区的内存池
__align(4) char node_buff[QUEUE_NODE_NUM][QUEUE_NODE_DATA_LEN] ;



/*环形缓冲队列*/

/**
  * @brief  初始化缓冲队列
  * @param  cb:缓冲队列结构体
  * @param  size: 缓冲队列的元素个数
  * @note 	初始化时还需要给cb->elems指针赋值
  */
void cbInit(QueueBuffer *cb, int size) 
{
    cb->size  = size;	/* maximum number of elements           */
    cb->read = 0; 		/* index of oldest element              */
    cb->write   = 0; 	 	/* index at which to write new element  */
//    cb->elems = (uint8_t *)calloc(cb->size, sizeof(uint8_t));  //elems 要额外初始化
}
 
//(此函数改成了宏，在头文件)
/**
  * @brief  输出缓冲队列当前的状态信息
  * @param  cb:缓冲队列结构体
  */
//void cbPrint(QueueBuffer *cb) 
//{
//    DATA_QUEUE_LOG("size=0x%x, read=%d, write=%d\n", cb->size, cb->read, cb->write);
//	  DATA_QUEUE_LOG("size=0x%x, read_using=%d, write_using=%d\n", cb->size, cb->read_using, cb->write_using);
//}
 
/**
  * @brief  判断缓冲队列是(1)否(0)已满
  * @param  cb:缓冲队列结构体
  */
int cbIsFull(QueueBuffer *cb) 
{
    return cb->write == (cb->read ^ cb->size); /* This inverts the most significant bit of read before comparison */ 
}
 
/**
  * @brief  判断缓冲队列是(1)否(0)全空
  * @param  cb:缓冲队列结构体
  */		
int cbIsEmpty(QueueBuffer *cb) 
{
    return cb->write == cb->read; 
}

/**
  * @brief  对缓冲队列的指针加1
  * @param  cb:缓冲队列结构体
  * @param  p：要加1的指针
  * @return  返回加1的结果
  */	
int cbIncr(QueueBuffer *cb, int p) 
{
    return (p + 1)&(2*cb->size-1); /* read and write pointers incrementation is done modulo 2*size */
}
 
/**
  * @brief  获取可写入的缓冲区指针
  * @param  cb:缓冲队列结构体
  * @return  可进行写入的缓冲区指针
  * @note  得到指针后可进入写入操作，但写指针不会立即加1，
           写完数据时，应调用cbWriteFinish对写指针加1
  */
QUEUE_DATA_TYPE* cbWrite(QueueBuffer *cb) 
{
    if (cbIsFull(cb)) /* full, overwrite moves read pointer */
    {
			return NULL;
		}		
		else
		{
			//当wriet和write_using相等时，表示上一个缓冲区已写入完毕，需要对写指针加1
			if(cb->write == cb->write_using)
			{
				cb->write_using = cbIncr(cb, cb->write); //未满，则增加1
			}
		}
		
	return  cb->elems[cb->write_using&(cb->size-1)];
}



/**
  * @brief 数据写入完毕，更新写指针到缓冲结构体
  * @param  cb:缓冲队列结构体
  */
void cbWriteFinish(QueueBuffer *cb)
{
    cb->write = cb->write_using;
}
 
/**
  * @brief  获取可读取的缓冲区指针
  * @param  cb:缓冲队列结构体
  * @return  可进行读取的缓冲区指针
  * @note  得到指针后可进入读取操作，但读指针不会立即加1，
					 读取完数据时，应调用cbReadFinish对读指针加1
  */
QUEUE_DATA_TYPE* cbRead(QueueBuffer *cb) 
{
		if(cbIsEmpty(cb))
			return NULL;
		
	//当read和read_using相等时，表示上一个缓冲区已读取完毕(即已调用cbReadFinish)，
	//需要对写指针加1
	if(cb->read == cb->read_using)	
		cb->read_using = cbIncr(cb, cb->read);
	
	return cb->elems[cb->read_using&(cb->size-1)];
}


/**
  * @brief 数据读取完毕，更新读指针到缓冲结构体
  * @param  cb:缓冲队列结构体
  */
void cbReadFinish(QueueBuffer *cb) 
{	
		//重置当前读完的数据节点的长度
		cb->elems[cb->read_using&(cb->size-1)]->len = 0;
	
    cb->read = cb->read_using;
}



//队列的指针指向的缓冲区全部销毁
void camera_queue_free(void)
{
    uint32_t i = 0;

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        if(node_data[i].head != NULL)
        {
					//若是动态申请的空间才要free
//            free(node_data[i].head);
            node_data[i].head = NULL;
        }
    }

    return;
}


/**
  * @brief  缓冲队列初始化，分配内存,使用缓冲队列时，
  * @param  无
  * @retval 无
  */
void rx_queue_init(void)
{
  uint32_t i = 0;

  memset(node_data, 0, sizeof(node_data));
		 
	/*初始化缓冲队列*/
	cbInit(&rx_queue,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_data[i].head = node_buff[i];
        
        /*初始化队列缓冲指针，指向实际的内存*/
        rx_queue.elems[i] = &node_data[i];
        
        DATA_QUEUE_LOG("node_data[i].head=0x%x,\r\nrx_queue.elems[i] =0x%x", (uint32_t)node_data[i].head,(uint32_t)rx_queue.elems[i]->head);

        memset(node_data[i].head, 0, QUEUE_NODE_DATA_LEN);
    }
		
	cbPrint(rx_queue);	
}



/**
  * @brief  往队列中写入数据的样例
  */
void push_data_to_queue(char *src_dat,uint16_t src_len)
{
	QUEUE_DATA_TYPE *data_p;
	uint8_t i;
	
	for(i=0;i<src_len;i++)
	{
		/*获取写缓冲区指针，准备写入新数据*/
		data_p = cbWrite(&rx_queue);
		
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		
			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->head + i) = src_dat[i];
				data_p->len++;
			printf("\r\ndata_p->len =%d",data_p->len);
		}else return;	
		
		cbPrint(rx_queue);	
	}	
	
	/*写入缓冲区完毕*/
	cbWriteFinish(&rx_queue);
	
	cbPrint(rx_queue);	

}


/**
  * @brief  从队列中取数据的样例
  */
void pull_data_from_queue(void)
{
	QUEUE_DATA_TYPE *rx_data;	
		
	/*从缓冲区读取数据，进行处理，*/
	rx_data = cbRead(&rx_queue); 

	if(rx_data != NULL)//缓冲队列非空
	{		
		//加上字符串结束符，方便直接输出字符串
		*(rx_data->head+rx_data->len) = '\0';
		
		QUEUE_DEBUG("接收到的数据：%s",rx_data->head);
		QUEUE_DEBUG_ARRAY((uint8_t*)rx_data->head,rx_data->len);

		//使用完数据必须调用cbReadFinish更新读指针
		cbReadFinish(&rx_queue);
	}
}


