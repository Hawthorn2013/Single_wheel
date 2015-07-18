#define __STEPMOTOR_C_
#include "includes.h"

int stepspeed=30;
BYTE g_Control;



void Pit_1ms(void)
{
	static int timer=0;
	static int ABCD=0;
	static int count=0;
	g_Control=1;
	count++;
	if(count>=10)
	{
		get_speed_now();
		count=0;
	}
	
	angle_read(AngleResult_balance); //angleread()暂时使用PB10, PB11，与前后一样
	set_speed_pwm_balance();
	BalanceControl();
	if(AngleCalculate[0]<20&&AngleCalculate[0]>-20)
	{ 
		//LCD_PrintoutInt(65, 2, angle_pwm);
		motor_control_balance();
	} 
	else
		set_motor_pwm(0);
	
//	if(timer==stepspeed)
//	{
//		if(ABCD==0)
//		{
//			Motor_A();
//		}
//		else if(ABCD==1)
//		{
//			Motor_B();
//		}
//		else if(ABCD==2)
//		{
//			Motor_C();
//		}
//		else if(ABCD==3)
//		{
//			Motor_D();
//		}
//		
//		timer=0;
//		ABCD++;
//		ABCD=ABCD%4;
//	}
//	else
//		timer++;
//		
	PIT.CH[2].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
}
/*-----------------------------------------------------------------------*/
/* 初始化步进电机ABCD相                                                         */
/* PB11 PB12 PB13 PB15                                                   */
/*-----------------------------------------------------------------------*/
void init_Stepmotor(void)
{
 	SIU.PCR[16].R = 0x0203;	/* A相 PB0 */
  	SIU.PCR[17].R = 0x0203; /* B相 PB1 */
 	SIU.PCR[72].R = 0x0203; /* C相 PE8 */
	SIU.PCR[73].R = 0x0203;	/* D相 PE9 */	

	Motor_stop();
}
void Motor_stop(void)
{
	SMotor_A=1;
	SMotor_B=1;
	SMotor_C=1;
	SMotor_D=1;
}
void Motor_A(void)
{
	SMotor_A=0;
	SMotor_B=1;
	SMotor_C=1;
	SMotor_D=1;
}
void Motor_B(void)
{
	SMotor_A=1;
	SMotor_B=0;
	SMotor_C=1;
	SMotor_D=1;
}
void Motor_C(void)
{
	SMotor_A=1;
	SMotor_B=1;
	SMotor_C=0;
	SMotor_D=1;
}
void Motor_D(void)
{
	SMotor_A=1;
	SMotor_B=1;
	SMotor_C=1;
	SMotor_D=0;
}
