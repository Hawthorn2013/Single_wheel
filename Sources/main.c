﻿#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;

void main(void)
{
//	float W[16];
//	float q0[4]={1,0,0,0};	//四元数
//	float q1[4];
//	float sita,gama,fai;
//	float Cbn[9];
//	int i;
	uint8_t Data[41];
	init_all_and_POST();
	D0=0;
	for(;;)
	{
		GY953_multi_Read(Data);
//		Read_GYalldata(Data);
		send_data2PC(3, ACC_TYPE, Data);
		//send_data2PC(3, GYR_TYPE, Data);
		send_data2PC(3, MAG_TYPE, Data);
		send_data2PC(3,ANGLE_TYPE,Data);
//		serial_port_1_TX(0xAA);
		delay_ms(5);
		
		
//		Read_GYRO_ACC(Data);
//		W_get(W);
//		Runge_Kutta(q0,50,W,q1);
//		for(i=0;i<4;i++)
//			q0[i]=q1[i];
//		//angle_get(q1,sita,gama,fai);
//		//Cbn=Cb_n(q0);
	}
	
//	static BYTE dall[2]={9,8};
//	init_all_and_POST();
//	set_speed_target(20);
//	for(;;)
//	{
//
//		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
//		{
//			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
//			
//			execute_remote_cmd(remote_frame_data+2);
//			
//			D8=~D8;
//		}
////		delay_ms(10);
//
//#if 1		
//		if(g_Control)
//		{
//			g_Control=0;
//			count++;
//			speed_period++;
//			angle_read(AngleResult);
//			set_speed_pwm();
//			AngleControl();
//			
//			LCD_PrintoutInt(0, 0, angle_data.PITCH_angle_zero);
////			LCD_PrintoutInt(0, 2, AngleCalculate[1]);
////			LCD_PrintoutInt(0, 4, AngleCalculate[0]);
//			LCD_PrintoutInt(0, 2, angle_data.YAW_anglespeed_zero);
////			LCD_PrintoutInt(64, 2, AngleCalculate[0]);
//
//			if(AngleCalculate[0]<20&&AngleCalculate[0]>-20)
//			{ 
//				motor_control();
//			} 
//			else
//			{
//				set_motor_pwm(0);
//			}
//			if(count==4)
//			{
//				SpeedCountFlag++;
//				get_speed_now();
//				if(SpeedCountFlag>=20) 
//				{
//					set_speed_PID();
//					contorl_speed_encoder_pid();
//					speed_period=0;
////					LCD_PrintoutInt(65, 4, data_encoder.speed_real);
//					SpeedCountFlag=0;
//				}
//			}
//			else if(count==5)
//			{
//				send_data2PC(ENC03,GYR_TYPE,dall);
//				count=0;
//			}
//		}
//#endif
//	}

}
