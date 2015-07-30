#include "includes.h"

uint8_t GY953_Data[41];
BYTE count=0;
BYTE  SpeedCountFlag=0;
int stepcount=0;

void main(void)
{
	float q0,q1,q2,q3;
	init_all_and_POST();
	
	D0=0;
	D1=0;
	D2=0;
	D3=0;
	
	//GY953_Four(&q0,&q1,&q2,&q3,GY953_Data);
	for(;;)
	{
		Read_GYalldata(GY953_Data);
//		send_data2PC(3,ACC_TYPE,Data);
//		send_data2PC(3,MAG_TYPE, Data);
//		send_data2PC(3,ANGLE_TYPE,Data);
//		send_data2PC(3,FOUR_TYPE,Data);

		delay_ms(5);
	}
	
}

