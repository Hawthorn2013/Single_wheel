#include "includes.h"

int32_t Data[6];
float a=0;
float v=0;
float s=0;

#define g 9.8
#define stime 0.02		//采样时间 单位s

void Mode0_DebugCamera(void);
void Mode1_SendVideo(void);
void Mode2_GO(void);
void Mode3_Andriod(void);

void main(void)
{
	int i=0;
	int32_t xdev;
	init_all_and_POST();
	D0=0;
	deviation_adjust_accx(&xdev);
	for(;;)
	{

		Read_GYRO_ACC(Data);
		a=-(Data[3]-xdev)*2*g/65536;
		v=v+a*stime;
		s=s+v*stime;
		if(s>=2)
			D1=0;
//		for(i=0;i<6;i++)
//		{
//			LCD_Write_Num(85,i,Data[i],5);
//		}
		{
			LCD_Write_Num(45,1,Data[3]-xdev,5);
			LCD_Write_Num(45,2,Data[4],5);
			LCD_Write_Num(45,3,Data[5],5);
			LCD_PrintoutFloat(45, 4, v);
			LCD_PrintoutFloat(45, 6, s);
		}
		i++;
		delay_ms(20);
	}
}
