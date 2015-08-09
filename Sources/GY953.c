#define __GY953_C_
#include "includes.h"

/*------------------------------------------------------------------------------*/
/* 电子罗盘GY953初始化
/*------------------------------------------------------------------------------*/
void init_GY953(void)
{
	BYTE Precision[3];
	GY953_Write(SET_A,0x7D);		//开启磁场计 陀螺仪 加速度计 设置输出速率200HZ
	GY953_Write(CONTROL_B,0x11);		//自检 校准 高位置1可恢复出厂设置
	GY953_Write(STATE_D,0x0D);		//设置模块量程
	
	GY953_Write(CONTROL_B,0x15);		//加陀校准
	Read_Precision(Precision);
	serial_port_1_TX_array(Precision,3);
	

}

/*------------------------------------------------------------------------------*/
/* 读取三计校准精度
/* 有数据更新返回1 无新数据返回0 
/* Data[0]加速度计	Data[1]陀螺计	Data[2]磁场计 Data[3]数据输出速率	(低0~4高)
/*------------------------------------------------------------------------------*/
int Read_Precision(BYTE* Data)
{
	BYTE state;
	BYTE set;
	while(!GY953_Read(STATE_C,&state)){};
	while(!GY953_Read(SET_A,&set)){};
	if(state>>7)
	{
		Data[0]=state&0x30;
		Data[0]>>=4;	
		Data[1]=state&0x0C;
		Data[1]>>=2;
		Data[2]=state&0x03;
		Data[3]=set&0x07;
		return 1;
	}
	else
		return 0;
}

/*------------------------------------------------------------------------------*/
/* 读取全部数据
/* 有数据更新返回1 无新数据返回0 
/* GY953 Data[41]
/* 加速度Data[2~7]角速度 Data[8~9]X[10~11]Y[12~13]Z；
/* Data[高20~21低]横滚角； Data[22~23]俯仰角；Data[24~25]航向角；Data[35]精度；
/* Data[36~39]加速度计6个数据寄存器数据累加和 、陀螺仪、磁力计、欧拉角
/*------------------------------------------------------------------------------*/
int Read_GYalldata(BYTE* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	uint8_t reg=0x01;
	int i=0;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0xC0;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	for(i=0;i<41;i++)
	{
		tmp_tx = 0x28080000|0xff;
		DSPI_1.PUSHR.R = tmp_tx;
		while(!DSPI_1.SR.B.TCF){}
		tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
		DSPI_1.SR.B.TCF = 1;
		
		Data[i]=tmp_rx;	
		reg++;
	}

	return 1;
}
/*------------------------------------------------------------------------------*/
/* GY953寄存器一次读取 一次8bit
/* 成功返回1 
/*------------------------------------------------------------------------------*/
int GY953_Read(uint8_t reg,uint8_t* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0x80;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;

	tmp_tx = 0x28080000|0xff;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	*Data=tmp_rx;
	return 1;
}

/*------------------------------------------------------------------------------*/
/* GY953寄存器连续一次性读取
/* 成功返回1 
/*------------------------------------------------------------------------------*/
int GY953_multi_Read(uint8_t* Data,uint8_t reg)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	int i=0;
	
	for(i=0;i<41;i++)
	{
		tmp_tx = 0xA8080000|(reg&0x3F)|0xC0;
		DSPI_1.PUSHR.R = tmp_tx;
		while(!DSPI_1.SR.B.TCF){}
		tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
		DSPI_1.SR.B.TCF = 1;

		tmp_tx = 0x28080000|0xff;
		DSPI_1.PUSHR.R = tmp_tx;
		while(!DSPI_1.SR.B.TCF){}
		tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
		DSPI_1.SR.B.TCF = 1;
		
		Data[i]=tmp_rx;	
		reg++;
	}

	return 1;
}
/*------------------------------------------------------------------------------*/
/* GY953寄存器写入 一次8bit
/* 成功返回1 
/*------------------------------------------------------------------------------*/
int GY953_Write(uint8_t reg,uint8_t Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0x40;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = 0x28080000|Data;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return 1;
}

int GY953_Four(float *q0,float *q1,float *q2,float *q3,uint8_t *Data)
{
	int16_t q00,q11,q22,q33;
	q00=q00|Data[26];
	q00=q00<<8|Data[27];
	//LCD_Write_Num(80,1,q00,5);
	*q0=q00/10000;
	
	q11=q11|Data[28];
	q11=q11<<8|Data[29];
	//LCD_Write_Num(80,2,q11,5);
	*q1=q11/10000;
	
	q22=q22|Data[30];
	q22=q22<<8|Data[31];
	//LCD_Write_Num(80,3,q22,5);
	*q2=q22/10000;
	
	q33=q33|Data[32];
	q33=q33<<8|Data[33];
	//LCD_Write_Num(80,4,q33,5);
	*q3=q33/10000;
	
	return 1;
}

int Wbn_get(float Wbn[],uint8_t *Data)
{
	float q0,q1,q2,q3;
	GY953_Four(&q0,&q1,&q2,&q3,Data);
	
	Wbn[0]=q0*q0+q1*q1-q2*q2-q3*q3;
	Wbn[1]=2*(q1*q2+q0*q3);
	Wbn[2]=2*(q1*q3-q0*q2);
	Wbn[3]=2*(q1*q2-q0*q3);
	Wbn[4]=q0*q0-q1*q1+q2*q2-q3*q3;
	Wbn[5]=2*(q2*q3+q0*q1);
	Wbn[6]=2*(q1*q3+q0*q2);
	Wbn[7]=2*(q2*q3-q0*q1);
	Wbn[8]=q0*q0-q1*q1-q2*q2+q3*q3;
}

int Acc_get(int16_t Acc[],uint8_t *Data)
{
	int16_t ax,ay,az;
	
	ax=ax|Data[2];
	ax=(ax<<8)|Data[3];
	
	ay=ay|Data[4];
	ay=(ay<<8)|Data[5];
	
	az=az|Data[6];
	az=(az<<8)|Data[7];
	
	Acc[0]=ax;
	Acc[1]=ay;
	Acc[2]=az;
	
	return 1;
}

int Dev_get(int16_t *dev)
{
	int32_t sum=0;
	int16_t d;
	uint8_t Acc_X_H,Acc_X_L;
	int16_t Acc_X=0;
	int i;
	for(i=0;i<5000;i++)
	{
		GY953_Read(0x03,&Acc_X_H);
		GY953_Read(0x04,&Acc_X_L);
		
		Acc_X=Acc_X|Acc_X_H;
		Acc_X=(Acc_X<<8)|Acc_X_L;
		
		sum+=Acc_X;
		
		delay_us(200);
	}
	
	d=sum/5000;
	
	*dev=d;
	
	return 1;
}

