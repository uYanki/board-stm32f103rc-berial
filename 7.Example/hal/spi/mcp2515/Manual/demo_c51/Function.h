#ifndef __FUNCTION_h__
#define __FUNCTION_h__


#include "reg51.h"

//*************************ÑÓÊ± n*1ms********************
void delay1ms(unsigned int j)
{
  unsigned int i;
  for (;j>0;j--)
  {
    for (i=0;i<124;i++)
    {
      ;
    }
  }
}

#endif
