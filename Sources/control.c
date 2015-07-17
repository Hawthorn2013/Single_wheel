#define __CONTROL_C_
#include "includes.h"


int g_f_pit = 0;
int g_f_enable_mag_steer_control = 0;
int g_f_enable_speed_control = 0;	/* 启用闭环速度控制标志位 */
int g_f_enable_pwm_control = 0;	/* 启用开环速度控制标志位 */
int speed = 0;
int update_steer_helm_basement_to_steer_helm(void);

int counter=0;

//速度控制全局变量
static float d_speed_pwm=0;
static float speed_pwm=0;
extern unsigned char g_nSpeedControlPeriod;
//角度控制全局变量
float fDelta;
float g_fCarAngle;
float g_fGyroscopeAngleSpeed;
float g_fGyroscopeTurnSpeed;
float CarAngleInitial=0;
float CarAnglespeedInitial=0;
extern float  AngleCalculate[4];

// float AngleControlOutMax=0.2, AngleControlOutMin=-0.2;
float  angle_pwm;
static int new_speed_pwm=0;
static int old_speed_pwm=0;
BYTE speed_period=0;





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
	data_encoder.is_forward = SIU.GPDI[28].B.PDI;//PC14
	data_encoder.cnt_old = data_encoder.cnt_new;
	data_encoder.cnt_new = (WORD)EMIOS_0.CH[24].CCNTR.R;//PD12
	if (data_encoder.cnt_new >= data_encoder.cnt_old)
	{
		data_encoder.speed_now = data_encoder.cnt_new - data_encoder.cnt_old;
	}
	else
	{
		data_encoder.speed_now = 0xffff - (data_encoder.cnt_old - data_encoder.cnt_new);
	}
	if(data_encoder.is_forward==0) 
		data_encoder.speed_real = 0 - data_encoder.speed_now;
	else 
		data_encoder.speed_real = data_encoder.speed_now;
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
	set_motor_pwm(motor_pwm);
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
	return e0;
	
}

/*-----------------------------------------------------------------------*/
/* PID速度控制                                                                       */
/* 有问题找叶川                                                                      */                                                          
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_pid(void)
{
	int error=0;
	static SWORD error_last=0;
	static SWORD sum_error=0;
	error_last = error;
	error = data_speed_settings.speed_target - data_encoder.speed_real;
	
	old_speed_pwm = new_speed_pwm;
	new_speed_pwm=(SWORD)(data_speed_pid.p*(error));       //P控制
	new_speed_pwm+=(SWORD)(data_speed_pid.d*(error-error_last));  //I控制
	sum_error+=error;
	if(sum_error>1000) sum_error=1000;
	if(sum_error<1000) sum_error=-1000;
	new_speed_pwm+=(SWORD)(data_speed_pid.i*(sum_error));		
	
	if(new_speed_pwm>1000)
		new_speed_pwm=1000;
	if(new_speed_pwm<-1000)
		new_speed_pwm=-1000;   //限制pwm变化量
	
	LCD_PrintoutInt(65, 4, error);
	LCD_PrintoutInt(65, 6, new_speed_pwm);
}
void set_speed_pwm(void)
{
	d_speed_pwm = new_speed_pwm - old_speed_pwm;
	speed_pwm = (d_speed_pwm/100)*(speed_period)+old_speed_pwm;
}

/*-----------------------------------------------------------------------*/
/* 设置目标速度                                                                      */
/*-----------------------------------------------------------------------*/
void set_speed_target(SWORD speed_target)
{
	data_speed_settings.speed_target = speed_target;
}


/*-----------------------------------------------------------------------*/
/* 设置平衡轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm2_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_2_pwm = speed_pwm;
}

/*-----------------------------------------------------------------------*/
/* 设置转向轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm3_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_3_pwm = speed_pwm;
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
