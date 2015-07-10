#include "includes.h"

int32_t Data[6];

void Mode0_DebugCamera(void);
void Mode1_SendVideo(void);
void Mode2_GO(void);
void Mode3_Andriod(void);

void main(void)
{
		init_all_and_POST();
		D0=0;
		for(;;)
		{
			delay_ms(20);
			Read_GYRO_ACC(Data);
			//LCD_Write_Num(105,2,a,2)
		}
}

