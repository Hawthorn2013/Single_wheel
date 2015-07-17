/*
 * analyse.c
 *
 *  Created on: Jul 13, 2015
 *      Author: Paul
 */
#include "includes.h"
#define g (9.8)
#define stime (0.001)		//采样时间 单位s

int32_t Data[6];	//陀螺仪，加速度计数据

int32_t xdev;	//零点偏差均值
int32_t ydev;	//零点偏差均值
int32_t zdev;	//零点偏差均值
int32_t xA=400;		//零点偏差幅值
float xa=0;		//x轴方向加速度
float xv=0;		//y轴方向加速度
float xs=0;		//z轴方向加速度

void distance(void)
{
	int32_t temp;
	const float range=65536;
	Read_GYRO_ACC(Data);	//读取陀螺仪数据
	
	temp=(Data[3]-xdev)/xA;
	xa=-temp*xA*4*g;	//对x方向加速度滤波
	xa=xa/range;

	xv=xv+xa*stime;			//求x方向速度
	xs=xs+xv*stime;			//求x方向距离

	if(xs>=2)				//2米标志
		D1=0;
	
	//显示屏显示
	LCD_Write_Num(80,1,Data[3],5);
//	LCD_Write_Num(45,2,Data[4],5);
//	LCD_Write_Num(45,3,Data[5],5);
	LCD_PrintoutFloat(45, 2, xa);
	LCD_PrintoutFloat(45, 4, xv);
	LCD_PrintoutFloat(45, 6, xs);
	
	delay_ms(1);//每5ms采样一次，与stime对应  （会不会有误差？）
}

void filter_display(void)
{
	int32_t Data[6];
	int32_t temp;
	int16_t temp1;
	uint8_t Data_H;		
	uint8_t Data_L;
	int32_t xa_1;
	int i=0;
	//Read_GYRO_ACC(Data);
	while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[3]=temp1;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(i++);
	serial_port_1_TX(0xAA);
	
	temp=(Data[3]-xdev)/xA;
	xa_1=temp*xA;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(i++);
	serial_port_1_TX(0xBB);

	i++;
	delay_ms(5);
}

void acc_display(uint32_t* i)	//i为循环次数,滤波后
{
	int32_t Data[6];
	int32_t temp;
	int16_t temp1;
	uint8_t Data_H;		
	uint8_t Data_L;
	int32_t xa_1;
	uint32_t ii=*i;
	
	while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[3]=temp1;
	
	temp=(Data[3]-xdev)/xA;
	xa_1=temp*xA+xdev;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii);
	serial_port_1_TX(0xAA);
	
	while(!Reg_Read(ACCEL_YOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_YOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[4]=temp1;
	
	temp=(Data[4]-ydev)/xA;
	xa_1=temp*xA+ydev;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii);
	serial_port_1_TX(0xBB);
	
	while(!Reg_Read(ACCEL_ZOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_ZOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[5]=temp1;
	
	temp=(Data[5]-zdev)/xA;
	xa_1=temp*xA+zdev;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii++);
	serial_port_1_TX(0xCC);
	
	
	*i=ii;
}

void temp_analyse(uint32_t* i)
{
	uint8_t Data_H;		
	uint8_t Data_L;
	uint32_t ii=*i;
	
	while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii);
	serial_port_1_TX(0xAA);
	
	while(!Reg_Read(TEMP_OUT_H,&Data_H)){}
	while(!Reg_Read(TEMP_OUT_L,&Data_L)){}
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii++);
	serial_port_1_TX(0xBB);
	
	*i=ii;
}
