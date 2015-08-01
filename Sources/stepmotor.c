#define __STEPMOTOR_C_
#include "includes.h"

int stepspeed=1000;
BYTE g_Control;
int timer=0;
int ABCD=0;

void Pit_1ms(void) 
{

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
			if(ABCD==3)
			{
				Motor_D();
			}
			else if(ABCD==2)
			{
				Motor_C();
			}
			else if(ABCD==1)
			{
				Motor_B();
			}
			else if(ABCD==0)
			{
				Motor_A();
			}
			timer=0;
			ABCD--;
			ABCD+=4;
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
	if(num==150)
	{
		stepspeed=-7;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;
	}
	if(num==250)
	{
		stepspeed=4;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;
	}
	if(num==360)
	{
		stepspeed=-5;
		timer=0;
		ABCD+=2;  //	原本的写法是两句  ABCD-=2;ABCD+=4;
		ABCD=ABCD%4;
	}
	if(num==500)
	{
		stepspeed=8;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
	if(num==800)
	{
		stepspeed=-4;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
	if(num==1100)
	{
		stepspeed=6;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
	if(num==1250)
	{
		stepspeed=-3 ;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
	if(num==1550)
	{
		stepspeed=5;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
	if(num==1750)
	{
		stepspeed=-8;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
	if(num==2100)
	{
		stepspeed=3;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
	if(num==2300)
	{
		stepspeed=-4;
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;				
	}
}

void stepmotor_balance(void)
{
	/*
	//步进电机调平衡 ，最大加速度
	if(AngleCalculate[2]>0)
	{
		stepspeed=3; //正负待定  3为最大转速
		timer=0;
		ABCD+=2;
		ABCD=ABCD%4;
	}  
	else if(AngleCalculate[2]<0)
	{
		stepspeed=-3;  //正负待定
		timer=0;
		ABCD-=2;
		ABCD+=4;
		ABCD=ABCD%4;
	}
	else
		stepspeed=1000;  //1000对应静止
	*/
	/*
	float fDelta_balance;
	float g_fCarAngle_balance;
	float g_fGyroscopeAngleSpeed_balance;
	float g_fGyroscopeTurnSpeed_balance;
	float CarAngleInitial_balance=0;
	float CarAnglespeedInitial_balance=0;
	
	float delta_angle_balance;
	float delta_anglespeed_balance;
	float temp_angle_balance, temp_anglespeed_balance;
	float currentanglespeed_balance, lastanglespeed_balance=0;
	float last_angle_balance=0;
	angle_calculate();
	g_fCarAngle_balance= AngleCalculate[2];
	g_fGyroscopeAngleSpeed_balance= -AngleCalculate[3]; 
	
	temp_angle_balance=CarAngleInitial_balance - g_fCarAngle_balance;
	temp_anglespeed_balance= CarAnglespeedInitial_balance - g_fGyroscopeAngleSpeed_balance;
		  
	//	   if(temp_angle_balance<-15)
	//		   data_angle_pid.p=100; //100开环
	//	   else if(temp_angle_balance>=-15&temp_angle_balance<=0)
	//		   data_angle_pid.p=200; //200
	//	   else if(temp_angle_balance>0&temp_angle_balance<=15)
	//		   data_angle_pid.p=200;// 170   
	//	   else
	//		   data_angle_pid.p=100;  //100
	//	                                                    
	//	  
	//	   if(temp_anglespeed_balance>=50||temp_anglespeed_balance<=-50)
	//		   data_angle_pid.d=2;//0.3
	//	   else
	//		   data_angle_pid.d=0.5;//0.1
		  
	currentanglespeed_balance=g_fCarAngle_balance;
	delta_anglespeed_balance=currentanglespeed_balance-lastanglespeed_balance;
	lastanglespeed_balance=currentanglespeed_balance;
		  
	delta_angle_balance = data_angle_pid.p*(CarAngleInitial_balance - g_fCarAngle_balance);
	delta_angle_balance+=data_angle_pid.d*0.6*(CarAnglespeedInitial_balance - g_fGyroscopeAngleSpeed_balance);
	delta_angle_balance+=data_angle_pid.d*0.4*delta_anglespeed_balance;
	//delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle) /5000 +data_angle_pid.d*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed) /15000; // 1000 与10000是否根据实际需要调整 
	//angle_pwm=delta_angle;
		  
//	if(delta_angle>AngleControlOutMax)
//	delta_angle=AngleControlOutMax;
//	 else if(delta_angle<AngleControlOutMin)
//	delta_angle=AngleControlOutMin;
		  
	//angle_pwm_balance=delta_angle_balance;
	//ROLL_angle_pwm=delta_angle_balance;
	stepspeed-=(int)delta_angle_balance;
	*/
	
	
	
	
	
	
	
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

