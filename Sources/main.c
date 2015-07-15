#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;

void main(void)
{
	init_all_and_POST();
	set_speed_target(30);
	for(;;)
	{
	/*	delay_ms(20);
		while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
		while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
		Data_H=Data_H<<8;
		Data=Data_H|Data_L;*/

		if(g_Control)
		{
			D5=~D5;
			g_Control=0;
			count++;
			speed_period++;
//			angle_read(AngleResult);
			set_speed_pwm();
//			AngleControl();
//			LCD_PrintoutInt(0, 0, AngleResult[1]);
//			LCD_PrintoutInt(0, 2, AngleCalculate[1]);
//			LCD_PrintoutInt(0, 4, AngleCalculate[0]);
			if(AngleCalculate[0]<20&&AngleCalculate[0]>-20)
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
				get_speed_now();
				if(SpeedCountFlag>=20) 
				{
					//set_speed_PID();
					contorl_speed_encoder_pid();
					speed_period=0;
					LCD_PrintoutInt(65, 0, data_encoder.is_forward);
					LCD_PrintoutInt(65, 2, data_encoder.speed_real);
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
