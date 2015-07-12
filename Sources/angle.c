#define __ANGLE_C_
#include "includes.h"

unsigned int jos[4]={0};
unsigned int ADResult[4]={0};
unsigned int AngleResult[4]={0};
float AngleCalculate[4]={0};


/*-----------------------------------------------------------------------*/
/*-----------主陀螺仪模块读值函数-------------*/
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
	}
	ADResult[0]=5000/1024*(temp[0]/20);
	ADResult[1]=5000/1024*(temp[1]/20);

	
	for(i=0;i<4;i++)
		*AngleGra++=ADResult[i]; 
}

/*-----------------------------------------------------------------------*/
/*-----------主陀螺仪模块数据处理-------------*/
/*-----------陀螺仪精度0.67-----------------*/
/*-----------------------------------------------------------------------*/
void angle_calculate(void)
{
	AngleCalculate[0]=((float)AngleResult[0]-1848)/5.4; //陀螺仪的角度  前倾的角度//1380   
	AngleCalculate[1]=((float)AngleResult[1]-972)/0.67;//陀螺仪角速度
//	AngleCalculate[2]=((float)AngleResult[2]-1532)/5; // 转向角度
//	AngleCalculate[3]=((float)AngleResult[3]-1378)/0.67; //加速度z轴  //1380
}

