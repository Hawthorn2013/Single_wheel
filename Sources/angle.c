#define __ANGLE_C_
#include "includes.h"

unsigned int jos[4]={0};
unsigned int ADResult[4]={0};
unsigned int AngleResult[4]={0}; //前后角度
float AngleCalculate[4]={0};
unsigned int AngleResult_balance[4]={0}; //左右角度
float AngleCalculate_balance[4]={0};

/*-----------------------------------------------------------------------*/
/*-----------俯仰陀螺仪模块读值函数-------------*/
/*--------------角度角速度-----------------*/
/*-----------------------------------------------------------------------*/
void angle_read(unsigned int *AngleGra)
{
	int i;
	unsigned int temp[4]={0};
	for(i=0;i<20;i++)
	{
		jos[0]=(unsigned int)ADC.CDR[34].B.CDATA;
		temp[0]+=jos[0];//PB10
		jos[1]=(unsigned int)ADC.CDR[35].B.CDATA;
		temp[1]+=jos[1];//PB11
		jos[2]=(unsigned int)ADC.CDR[37].B.CDATA;
		temp[2]+=jos[2];
		jos[3]=(unsigned int)ADC.CDR[39].B.CDATA;
		temp[3]+=jos[3];
	}
	ADResult[0]=5000/1024*(temp[0]/20);
	ADResult[1]=5000/1024*(temp[1]/20);

	
	for(i=0;i<4;i++)
		*AngleGra++=ADResult[i]; 
}

/*-----------------------------------------------------------------------*/
/*-----------俯仰陀螺仪模块数据处理-------------*/
/*-----------陀螺仪精度0.67-----------------*/
/*-----------------------------------------------------------------------*/
void angle_calculate(void)
{
//	AngleCalculate[0]=((float)AngleResult[0]-angle_data.PITCH_angle_zero)/5.49; //陀螺仪的角度  前倾的角度//1380   
//	AngleCalculate[1]=((float)AngleResult[1]-angle_data.PITCH_anglespeed_zero)/0.67;//陀螺仪角速度

	AngleCalculate[0]=((float)AngleResult[0]-1500)/5.49; //暂时用作 左右角度  //1380   
	AngleCalculate[1]=((float)AngleResult[1]-1500)/0.67;//陀螺仪角速度
	
//	AngleCalculate[2]=((float)AngleResult[2]-1532)/5; // 转向角度
//	AngleCalculate[3]=((float)AngleResult[3]-1378)/0.67; //加速度z轴  //1380
}

/*-----左右陀螺仪数据处理---*/
void angle_calculate_balance(void)
{
	AngleCalculate_balance[0]=((float)AngleResult_balance[0]-angle_data.ROLL_angle_zero)/5.49; //陀螺仪的角度  前倾的角度//1380   
	AngleCalculate_balance[1]=((float)AngleResult_balance[1]-angle_data.ROLL_anglespeed_zero)/0.67;//陀螺仪角速度
}


void set_PITCH_angle_zero(DWORD zero)
{
	angle_data.PITCH_angle_zero = zero;
}

void set_PITCH_angle_speed_zero(DWORD zero)
{
	angle_data.PITCH_anglespeed_zero = zero;
}

void set_ROLL_angle_zero(DWORD zero)
{
	angle_data.ROLL_angle_zero = zero;
}

void set_ROLL_angle_speed_zero(DWORD zero)
{
	angle_data.ROLL_anglespeed_zero = zero;
}

void set_YAW_angle_zero(DWORD zero)
{
	angle_data.YAW_angle_zero = zero;
}

void set_YAW_angle_speed_zero(DWORD zero)
{
	angle_data.YAW_anglespeed_zero = zero;
}
