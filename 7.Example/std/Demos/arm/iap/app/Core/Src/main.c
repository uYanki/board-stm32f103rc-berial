#include "board.h"

#define TAG              "app"
#define DBG_LVL          SDK_DEBUG

int main(void)
{
	
	board_init();
   
    while (1) {
		  LED=1;
			delay_ms(1000);
			LED=0;
			delay_ms(1000);
    }

}
