#include "includes.h"

extern uint16_t Data=0x00;

void Mode0_DebugCamera(void);
void Mode1_SendVideo(void);
void Mode2_GO(void);
void Mode3_Andriod(void);

void main(void)
{
		uint16_t Data_H=0x0000;
		uint16_t Data_L=0x0000;
		init_all_and_POST();
		for(;;)
		{
			delay_ms(20);
			while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
			while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
			Data_H=Data_H<<8;
			Data=Data_H|Data_L;
		}
}

