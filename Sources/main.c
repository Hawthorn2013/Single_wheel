#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag;
extern uint16_t Data=0x00;

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
		
		if(g_Control)
		{
			g_Control=0;
			count++;
			angle_read(AngleResult);
			//angle_calculate();
			set_speed_pwm();
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
				get_speed_now();
				SpeedCountFlag++;
				if(SpeedCountFlag>=20) 
				{
					set_speed_PID();
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


