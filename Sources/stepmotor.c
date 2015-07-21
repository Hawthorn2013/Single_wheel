#define __STEPMOTOR_C_
#include "includes.h"

int stepspeed=1000;
BYTE g_Control;
int timer=0;
int ABCD=0;

void Pit_1ms(void) // 问题：timer一直是负值。考虑加串口记录四个口输出
{
<<<<<<< HEAD
	//static int timer=0;
	//static int ABCD=0;
	//static int count=0;
	g_Control=1;
	//count++;
	//stepmotor_rotate();
	
	if(stepspeed>0)
	{
		if(timer==stepspeed)
		{
			if(ABCD==0)
			{     
				Motor_A();
			}
		else if(ABCD==1)
			{
				Motor_B();
			}
		else if(ABCD==2)
			{
				Motor_C();
			}
		else if(ABCD==3)
			{
				Motor_D();
			}
		timer=0;
		ABCD++;
		ABCD=ABCD%4;
		}
		else
			timer++;
	}
	else if(stepspeed<0)            //stepspeed为负值
	{
		if(timer==stepspeed)
		{
			if(ABCD==0)
			{
				Motor_D();
			}
			else if(ABCD==1)
			{
				Motor_C();
			}
			else if(ABCD==2)
			{
				Motor_B();
			}
			else if(ABCD==3)
			{
				Motor_A();
			}
			timer=0;
			ABCD++;
			ABCD=ABCD%4;
		}
		else
			timer--;
	}
	
	
	
	/*
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
	*/
	


=======
//	static int timer=0;
//	static int ABCD=0;
	static int count=0;
	
	g_Control=1;
	count++;
	
	
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
>>>>>>> 8c625fc5adef5b34f082c5062cb56d0e19c3d5c6
	PIT.CH[2].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
}


void stepmotor_video(int num)
{
	//视频用步进电机转动
	if(num==1)
	{
		stepspeed=3;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;
	}
	if(num==1000)
	{
		stepspeed=10;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;
	}
	if(num==1700)
	{
		stepspeed=-5;
		timer=0;
		ABCD+=2;  //	原本的写法是两句  ABCD-=2;ABCD+=4;
		ABCD=ABCD%4;
	}
	if(num==2100)
	{
		stepspeed=-15;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
}

void stepmotor_balance(void)
{
	//步进电机调平衡 
	
	if(AngleResult_balance[0]>0)
	{
		stepspeed=3; //正负待定  3为最大转速
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;
	}  
	else if(AngleResult_balance[0]<0)
	{
		stepspeed=-3;  //正负待定
		timer=0;
		ABCD-=2;
		ABCD+=4;
		ABCD=ABCD%4;
	}
	else
		stepspeed=1000;  //1000对应静止
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

