#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;
int stepcount=0;

void main(void)
{
	uint8_t Data[41];
	init_all_and_POST();
	for(;;)
	{
		Read_GYalldata(Data);
		send_data2PC(3,ACC_TYPE,Data);
		send_data2PC(3, MAG_TYPE, Data);
		send_data2PC(3,ANGLE_TYPE,Data);
		delay_ms(5);
	}
	
}

