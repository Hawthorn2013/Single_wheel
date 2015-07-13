#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag;
extern uint16_t Data=0x00;

void main(void)
{
//	uint16_t Data_H=0x0000;
//	uint16_t Data_L=0x0000;
	init_all_and_POST();
	set_speed_target(0);
	for(;;)
	{
	/*	delay_ms(20);
		while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
		while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
		Data_H=Data_H<<8;
		Data=Data_H|Data_L;*/
		if(g_Control)
		{
			D6=~D6;
			g_Control=0;
			count++;
			set_speed_pwm();
			angle_read(AngleResult);
			angle_calculate();
			AngleControl();
			if(AngleCalculate[0]<27&&AngleCalculate[0]>-48)
			{ 
				motor_control();
			} 
			else
			{
				set_motor_pwm(0);
			}
			if(count==4)
			{
				SpeedCountFlag++;
				if(SpeedCountFlag>=2) 
				{
		//			LCD_PrintoutInt(5, 0,data_encoder.speed_now);
				//	set_speed_PID();
					contorl_speed_encoder_pid();
					SpeedCountFlag=0;
					
					
				}
			}
			else if(count==5)
			{
				count=0;
			}
		}
	}
}


