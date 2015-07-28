#define __GY953_C_
#include "includes.h"

/*------------------------------------------------------------------------------*/
/* 电子罗盘GY953初始化
/*------------------------------------------------------------------------------*/
void init_GY953(void)
{
	GY953_Write(SET_A,0x7D);		//开启磁场计 陀螺仪 加速度计 设置输出速率200HZ
	GY953_Write(CONTROL_B,0x1D);		//自检 校准 高位置1可恢复出厂设置
	GY953_Write(STATE_D,0x0D);		//设置模块精度
}

/*------------------------------------------------------------------------------*/
/* 读取三计校准精度
/* 有数据更新返回1 无新数据返回0 
/* Data[0]加速度计	Data[1]陀螺计	Data[2]磁场计	(低0~3高)
/*------------------------------------------------------------------------------*/
int Read_Precision(BYTE* Data)
{
	BYTE state;
	while(!GY953_Read(STATE_C,&state)){};
	if(state>>7)
	{
		Data[0]=state&0x30;
		Data[0]>>=4;	
		Data[1]=state&0x0C;
		Data[1]>>=2;
		Data[2]=state&0x03;
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
	BYTE data[]={0};
	BYTE reg=0x01;
	int i=0;
	for(i=0;i<41;i++)
	{
		while(!GY953_Read(reg,&data[i])){};
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
int GY953_multi_Read(uint8_t* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	uint8_t reg=0x01;
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

