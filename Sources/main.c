#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;
int stepcount=0;

void main(void)
{
	init_all_and_POST();
	set_speed_target(20);
	for(;;)
	{

<<<<<<< HEAD
		
		while(g_start_PCcontrol)
=======
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
>>>>>>> 8c625fc5adef5b34f082c5062cb56d0e19c3d5c6
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+2);
			
			D8=~D8;
		}
//		delay_ms(10);

#if 1		
		if(g_Control)
		{
			g_Control=0;
			count++;
			stepcount++;
//			LCD_PrintoutInt(64, 0, ABCD);
//			LCD_PrintoutInt(0, 0, SIU.GPDO[16].B.PDO);
//			LCD_PrintoutInt(0, 2, SIU.GPDO[17].B.PDO);
//			LCD_PrintoutInt(0, 4, SIU.GPDO[72].B.PDO);
//			LCD_PrintoutInt(0, 6, SIU.GPDO[73].B.PDO);

			
//			//步进电机调平衡
//			angle_read(AngleResult_balance);
//			stepmotor_balance();
			//视频用步进电机转动
			stepmotor_video(stepcount);
			
			speed_period++;
			angle_read(AngleResult);
			set_speed_pwm();
			AngleControl();
<<<<<<< HEAD
			//LCD_PrintoutInt(0, 0, AngleResult[1]);
			//LCD_PrintoutInt(0, 2, AngleCalculate[1]);
			//LCD_PrintoutInt(0, 4, AngleCalculate[0]);
			//LCD_PrintoutInt(64, 0, AngleResult[0]);
			//LCD_PrintoutInt(64, 2, AngleCalculate[0]);
=======
			
			LCD_PrintoutInt(0, 0, AngleResult[0]);
			LCD_PrintoutInt(64, 0, AngleResult[1]);
			LCD_PrintoutInt(0, 2, angle_data.PITCH_angle_zero);
			LCD_PrintoutInt(64, 2, angle_data.PITCH_anglespeed_zero);
			LCD_PrintoutInt(0,4, data_angle_pid.p);
			LCD_PrintoutInt(0, 6, data_angle_pid.d);

>>>>>>> 8c625fc5adef5b34f082c5062cb56d0e19c3d5c6
			if(AngleCalculate[0]<20&&AngleCalculate[0]>-20)
			{ 
				PITCH_motor_control();
			} 
			else
<<<<<<< HEAD
				set_motor_pwm(0);
		    
			
//			if(count==3)
//			{
//				BalanceControl(); //暂时与前后平衡共用电机接口，不能同时运行
//			}
			
=======
			{
				set_PITCH_motor_pwm(0);
			}

>>>>>>> 8c625fc5adef5b34f082c5062cb56d0e19c3d5c6
			if(count==4)
			{
				SpeedCountFlag++;
				get_speed_now();
				if(SpeedCountFlag>=20) 
				{
					set_speed_PID();
					contorl_speed_encoder_pid();
					speed_period=0;
//					LCD_PrintoutInt(0, 4, data_speed_settings.speed_target);
//					LCD_PrintoutInt(65, 4, data_encoder.speed_real);
					SpeedCountFlag=0;
				}
			}
			
			if(count==5)
			{
//				send_data2PC(ENC03,GYR_TYPE,dall);
				count=0;
			}
			if(stepcount==2500)
			{
				stepcount=0;
			}
		}
#endif
	}
}

