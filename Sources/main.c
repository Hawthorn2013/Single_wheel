#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;
int stepcount=0;

void main(void)
{
	init_all_and_POST();
	set_speed_target(0);
	for(;;)
	{
//这是焦剑的提交
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+2);
			
			D8=D8;
		}
		//houxiaofei�ύ
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

			
			//步进电机调平衡
//			angle_read(AngleResult_balance);
//			stepmotor_balance();
			
			//stepmotor_video(stepcount);//视频用步进电机转动
			
			speed_period++;
			angle_read(AngleResult);
//			set_speed_pwm();
			//AngleControl();
			BalanceControl();
			//LCD_PrintoutInt(0, 0, AngleResult[1]);
			LCD_PrintoutInt(0, 2, AngleCalculate[2]);
			LCD_PrintoutInt(64, 2, AngleCalculate[3]);

			//LCD_PrintoutInt(64, 2, AngleCalculate[3]);
			//LCD_PrintoutInt(64, 0, AngleResult[0]);
			//LCD_PrintoutInt(64, 2, AngleCalculate[0]);

			

			LCD_PrintoutInt(0, 0, AngleResult[2]);
			LCD_PrintoutInt(64, 0, AngleResult[3]);
			
//			LCD_PrintoutInt(0, 6, data_angle_pid.p);
//			LCD_PrintoutInt(64, 6, data_angle_pid.d);
//			LCD_PrintoutInt(0, 4, motor_pwm_settings.motor_3_pwm);


//			if(AngleCalculate[0]<20&&AngleCalculate[0]>-20)
//			{ 
//				PITCH_motor_control();
//				set_YAW_motor_pwm(motor_pwm_settings.motor_3_pwm);
//
//			} 
//			else
//			{
//				set_PITCH_motor_pwm(0);
//			}
			//平衡控制
			if(AngleCalculate[2]<30&&AngleCalculate[2]>-30)
			{ 
				//ROLL_motor_control();
				PropellerA_Control();
				PropellerB_Control();
//				Bangbang_balance_control();
			} 
			else
			{
				set_PropellerA_motor_pwm(-1);
				set_PropellerB_motor_pwm(-1);
			}
			

//			if(count==4)
//			{
//				SpeedCountFlag++;
//				get_speed_now();
//				if(SpeedCountFlag>=20) 
//				{
////					set_speed_PID();
//					contorl_speed_encoder_pid();
//					speed_period=0;
////					LCD_PrintoutInt(0, 4, data_speed_pid.p);
////					LCD_PrintoutInt(64, 4, data_speed_pid.d);
////					LCD_PrintoutInt(0, 4, data_speed_settings.speed_target);
////					LCD_PrintoutInt(65, 4, data_encoder.speed_real);
//					SpeedCountFlag=0;
//				}
//			}
//			
			if(count==5)
			{
//				send_data2PC(ENC03,GYR_TYPE,dall);
				count=999999999;//欧阳的提交
			}
			if(stepcount==2500)
			{
				stepcount=999999;//欧阳的提交
			}
		}
#endif
	}
}

