﻿#define __CONTROL_C_
#include "includes.h"


int g_f_pit = 0;
int g_f_enable_mag_steer_control = 0;
int g_f_enable_speed_control = 0;	/* 启用闭环速度控制标志位 */
int g_f_enable_pwm_control = 0;	/* 启用开环速度控制标志位 */
int speed = 0;
int update_steer_helm_basement_to_steer_helm(void);
int g_f_big_U=0;
int g_f_big_U_2=0;
int counter=0;

//速度控制全局变量
static float d_speed_pwm=0;
static float speed_pwm=0;
extern unsigned char g_nSpeedControlPeriod;
//平衡控制速度全局变量
static float d_speed_pwm_balance=0;
static float speed_pwm_balance=0;
extern unsigned char g_nSpeedControlPeriod_balance;
//角度控制全局变量
float fDelta;
float g_fCarAngle;
float g_fGyroscopeAngleSpeed;
float g_fGyroscopeTurnSpeed;
float CarAngleInitial=0;
float CarAnglespeedInitial=0;
extern float  AngleCalculate[4];
//左右平衡控制全局变量
float fDelta_balance;
float g_fCarAngle_balance;
float g_fGyroscopeAngleSpeed_balance;
float g_fGyroscopeTurnSpeed_balance;
float CarAngleInitial_balance=0;
float CarAnglespeedInitial_balance=0;
extern float  AngleCalculate_balance[4];


// float AngleControlOutMax=0.2, AngleControlOutMin=-0.2;
float  angle_pwm;
float angle_pwm_balance;

DWORD tmp_a, tmp_b;


/*-----------------------------------------------------------------------*/
/* 舵机初始化 	                                                                      */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* PIT中断处理函数        10ms                                                         */
/*-----------------------------------------------------------------------*/
void PitISR(void)
{
	get_speed_now();//光编读值
	/* 开始执行速度控制算法 */
	PIT.CH[1].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
}

/*-----------------------------------------------------------------------*/
/* 获得光编当前速度                                                                    */
/*-----------------------------------------------------------------------*/
void get_speed_now()
{
	WORD speed_now;
	data_encoder.is_forward = SIU.GPDI[28].B.PDI;//PC14
	data_encoder.cnt_old = data_encoder.cnt_new;
	data_encoder.cnt_new = (WORD)EMIOS_0.CH[24].CCNTR.R;//PD12
	if (data_encoder.cnt_new >= data_encoder.cnt_old)
	{
		speed_now = data_encoder.cnt_new - data_encoder.cnt_old;
	}
	else
	{
		speed_now = 0xffff - (data_encoder.cnt_old - data_encoder.cnt_new);
	}
	if(data_encoder.is_forward==0) 
		data_encoder.speed_now=0 - speed_now;
	else 
		data_encoder.speed_now= speed_now;
}
/*-----------------------------------------------------------------------*/
/* 设置电机PWM                                                                    */
/*-----------------------------------------------------------------------*/

void set_motor_pwm(int16_t motor_pwm)	//speed_pwm正为向前，负为向后
{
	if (motor_pwm>0)	//forward
	{
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}
		EMIOS_0.CH[21].CBDR.R = motor_pwm;
		EMIOS_0.CH[22].CBDR.R = 1;
		
	}
	else if (motor_pwm<0)	//backward
	{
		motor_pwm = 0-motor_pwm;
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}

		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = motor_pwm;	
	}
	else
	{
		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = 1;	
	}
}
void motor_control(void)
{
	int16_t motor_pwm;
//	motor_pwm=angle_pwm-speed_pwm;
	motor_pwm=speed_pwm;
//	motor_pwm=angle_pwm;
	set_motor_pwm(motor_pwm);
}
void motor_control_balance(void)
{
	int16_t motor_pwm_balance;
	motor_pwm_balance=angle_pwm_balance;
	set_motor_pwm(angle_pwm_balance);
}

/*-----------------------------------------------------------------------*/
/* BangBang速度控制                                                             */
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_bb(void)
{
	int32_t tmp_speed_now;
	
	
	if (data_encoder.is_forward)
	{
		tmp_speed_now = data_encoder.speed_now;
	}
	else
	{
		tmp_speed_now = 0 - data_encoder.speed_now;
	}
	
	if (tmp_speed_now > data_speed_settings.speed_target)
	{
		set_motor_pwm(0 - SPEED_PWM_MAX);
	}
	else if (tmp_speed_now < data_speed_settings.speed_target)
	{
		set_motor_pwm(SPEED_PWM_MAX);
	}
}

/*-----------------------------------------------------------------------*/
/* 前后角度控制                                                             */
/*-----------------------------------------------------------------------*/
void AngleControl(void)

{  float delta_angle;
   float delta_anglespeed;
   float temp_angle, temp_anglespeed;
   float currentanglespeed, lastanglespeed=0;
   float last_angle=0;
   angle_calculate();
   g_fCarAngle= AngleCalculate[0];
   g_fGyroscopeAngleSpeed= -AngleCalculate[1];
 // g_fGyroscopeTurnSpeed= AngleCalculateResult[2];
 
   temp_angle=CarAngleInitial - g_fCarAngle;
   temp_anglespeed= CarAnglespeedInitial - g_fGyroscopeAngleSpeed;
  
   if(temp_angle<-15)
	   data_angle_pid.p=100; //100开环
   else if(temp_angle>=-15&temp_angle<=0)
	   data_angle_pid.p=200; //200
   else if(temp_angle>0&temp_angle<=15)
	   data_angle_pid.p=200;// 170   
   else
	   data_angle_pid.p=100;  //100
                                                    
  
   if(temp_anglespeed>=50||temp_anglespeed<=-50)
	   data_angle_pid.d=2;//0.3
   else
	   data_angle_pid.d=0.5;//0.1
  
   currentanglespeed=g_fCarAngle;
   delta_anglespeed=currentanglespeed-lastanglespeed;
   lastanglespeed=currentanglespeed;
  
   delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle);
   delta_angle+=data_angle_pid.d*0.6*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed);
   delta_angle+=data_angle_pid.d*0.4*delta_anglespeed;
  //delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle) /5000 +data_angle_pid.d*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed) /15000; // 1000 与10000是否根据实际需要调整 
  //angle_pwm=delta_angle;
  
 /* if(delta_angle>AngleControlOutMax)
  delta_angle=AngleControlOutMax;
  else if(delta_angle<AngleControlOutMin)
  delta_angle=AngleControlOutMin;*/
  
  angle_pwm=delta_angle;
  
}

/*-----------------------------------------------------------------------*/
/* 左右平衡控制                                                             */
/*-----------------------------------------------------------------------*/
void BalanceControl(void)
{
	/*
	//用电位器调节转速，试验用
	int potential=1;
	int sss=1;
	potential=(int)ADC.CDR[33].B.CDATA;//PB9
	if(potential<0)
	{
		potential=0-potential;
	}
	if(potential>SPEED_PWM_MAX)
	{
		potential=SPEED_PWM_MAX;
	}
	
	if(potential<200)
		sss=200;
	else
		sss=1800;
	set_motor_pwm(sss);  
	*/
	
	float dta_angle;
	float dta_anglespeed;
	float temp_angle_balance, temp_anglespeed_balance;
	float currentanglespeed_balance, lastanglespeed_balance=0;
	float last_angle_balance=0;
	angle_calculate_balance();
	g_fCarAngle_balance= AngleCalculate_balance[0];
	g_fGyroscopeAngleSpeed_balance= -AngleCalculate_balance[1];
	 // g_fGyroscopeTurnSpeed= AngleCalculateResult[2];
	 
	temp_angle_balance=CarAngleInitial_balance - g_fCarAngle_balance;
	temp_anglespeed_balance= CarAnglespeedInitial_balance - g_fGyroscopeAngleSpeed_balance;
	  
	   if(temp_angle_balance<-15)
		   data_angle_pid.p=100; //100开环
	   else if(temp_angle_balance>=-15&temp_angle_balance<=0)
		   data_angle_pid.p=200; //200
	   else if(temp_angle_balance>0&temp_angle_balance<=15)
		   data_angle_pid.p=200;// 170   
	   else
		   data_angle_pid.p=100;  //100
	                                                    
	  
	   if(temp_anglespeed_balance>=50||temp_anglespeed_balance<=-50)
		   data_angle_pid.d=2;//0.3
	   else
		   data_angle_pid.d=0.5;//0.1
	  
	   currentanglespeed_balance=g_fCarAngle_balance;
	   dta_anglespeed=currentanglespeed_balance-lastanglespeed_balance;
	   lastanglespeed_balance=currentanglespeed_balance;
	  
	   dta_angle = data_angle_pid.p*(CarAngleInitial_balance - g_fCarAngle_balance);
	   dta_angle+=data_angle_pid.d*0.6*(CarAnglespeedInitial_balance - g_fGyroscopeAngleSpeed_balance);
	   dta_angle+=data_angle_pid.d*0.4*dta_anglespeed;
	  //delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle) /5000 +data_angle_pid.d*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed) /15000; // 1000 与10000是否根据实际需要调整 
	  //angle_pwm=delta_angle;
	  
	 /* if(delta_angle>AngleControlOutMax)
	  delta_angle=AngleControlOutMax;
	  else if(delta_angle<AngleControlOutMin)
	  delta_angle=AngleControlOutMin;*/
	  
	  angle_pwm_balance=dta_angle;
	
	
	
	
}

/*-----------------------------------------------------------------------*/
/* 获得速度偏差                                                                      */
/* 有问题找叶川                                                                      */
/*-----------------------------------------------------------------------*/
static SWORD get_e0()
{
	SWORD tmp_speed_now;
	SWORD e0;
	if (data_encoder.is_forward)
	{
		tmp_speed_now =(SWORD) data_encoder.speed_now;
	}
	else
	{
		tmp_speed_now = 0 - (SWORD) data_encoder.speed_now;
	}
	e0=data_speed_settings.speed_target-tmp_speed_now;
	LCD_PrintoutInt(5, 0,tmp_speed_now);
	LCD_PrintoutInt(0, 6,data_speed_settings.speed_target);
	return e0;
	
}

/*-----------------------------------------------------------------------*/
/* PID速度控制                                                                       */
/* 有问题找叶川                                                                      */                                                          
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_pid(void)
{
	SWORD d_speed_now= data_speed_settings.speed_target - data_encoder.speed_now_d ;
	static SWORD d_speed_last=0;
	static SWORD speed_i=0;
	static SWORD new_speed_pwm=0;
	static SWORD old_speed_pwm=0;
	new_speed_pwm=(SWORD)(data_speed_pid.p*(d_speed_now));       //P控制
	new_speed_pwm+=(SWORD)(data_speed_pid.d*(d_speed_now-d_speed_last));  //I控制
	speed_i+=d_speed_now;
	if(speed_i>70) speed_i=70;
	if(speed_i<-70) speed_i=-70;
	new_speed_pwm+=(SWORD)(data_speed_pid.i*(speed_i));		
	if(new_speed_pwm>1500)
		new_speed_pwm=1500;
	if(new_speed_pwm<-1500)
		new_speed_pwm=-1500;   //限制pwm变化量
	d_speed_last = d_speed_now;
	d_speed_pwm = new_speed_pwm - old_speed_pwm;
	old_speed_pwm = new_speed_pwm;
}
void set_speed_pwm(void)
{
	speed_pwm+=(d_speed_pwm/100);
	
//	LCD_PrintoutInt(0, 4,speed_pwm);
//	LCD_PrintoutInt(5, 6,d_speed_pwm/100);
}
void set_speed_pwm_balance(void)
{
	speed_pwm_balance+=(d_speed_pwm_balance/100);
}
/*-----------------------------------------------------------------------*/
/* 设置目标速度                                                                      */
/*-----------------------------------------------------------------------*/
void set_speed_target(SWORD speed_target)
{
	data_speed_settings.speed_target = speed_target;
}

/*-----------------------------------------------------------------------*/
/* 设置速度PID控制PID值 根据目标速度设置 speed_now                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_PID(void) 
{ 
	int speed_target=data_speed_settings.speed_target;
	int speed_now=data_speed_settings.speed_target_now;
	if(speed_target==0)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;         
	}
	else if(speed_target>=450&&speed_target<=500)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          

		if(speed_now<speed_target)
			speed_now+=25;
		else if(speed_now>speed_target)
			speed_now-=25;
		else
			speed_now=speed_target;
	}

	else if(speed_target>500&&speed_target<=560)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;         
		if(speed_now<speed_target)
			speed_now+=30;          //30可调 值越大初始时速度变化快
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 

	else if(speed_target>560&&speed_target<=620 )
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          
		if(speed_now<speed_target)
			speed_now+=30;
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 

	else
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          
		if(speed_now<speed_target)
			speed_now+=30;
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 

}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KP(float kp)
{
	data_speed_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制I值                                                             */
/*-----------------------------------------------------------------------*/
void set_speed_KI(float ki)
{
	data_speed_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KD(float kd)
{
	data_speed_pid.d = kd;
}



/*-----------------------------------------------------------------------*/
/* 获取两个周期计数的差值，常用故写成函数                               */
/*-----------------------------------------------------------------------*/
DWORD diff_time_basis_PIT(const DWORD new_time, const DWORD old_time)
{
	DWORD diff;
	
	if (new_time >= old_time)
	{
		diff = new_time - old_time;
	}
	else
	{
		diff = new_time + (0xFFFFFFFF- old_time);
	}
	
	return diff;
}
#if 0
int abs(int data)
{
	if (data<0)
		data=0-data;
	return data;
}
#endif
