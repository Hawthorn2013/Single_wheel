/*
 * analyse.c
 *
 *  Created on: Jul 13, 2015
 *      Author: Paul
 */
#include "includes.h"
#define g 9.8
#define stime 0.01		//采样时间 单位s

int32_t xdev;	//零点偏差均值
int32_t xA;		//零点偏差幅值
float xa=0;		//x轴方向加速度
float xv=0;		//y轴方向加速度
float xs=0;		//z轴方向加速度

void distance(void)
{
	int32_t Data[6];
	int32_t temp;
	int i=0;
	Read_GYRO_ACC(Data);
	temp=(Data[3]-xdev)/xA;
	xa=-temp*xA*g/17000;
	xv=xv+xa*stime;
	xs=xs+xv*stime;
	if(xs>=2)
		D1=0;
	LCD_Write_Num(45,1,Data[3],5);
//	LCD_Write_Num(45,2,Data[4],5);
//	LCD_Write_Num(45,3,Data[5],5);
	LCD_PrintoutFloat(45, 2, xa);
	LCD_PrintoutFloat(45, 4, xv);
	LCD_PrintoutFloat(45, 6, xs);
	i++;
	delay_ms(10);
}
