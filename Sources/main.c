#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;
int stepcount=0;

void main(void)
{
//	float Wbn[9];	//从载体坐标向导航坐标转换矩阵
//	float vnx=0;
//	float snx=0;
//	int16_t acc_b[3];
//	float Acc_b[3];
//	float Acc_n[3];
//	int i=0;
//	int16_t dev;
	init_all_and_POST();
	
	D0=0;
	D1=0;
	D2=0;
	D3=0;
	//Dev_get(&dev);
	for(;;)
	{
		Read_GYalldata(GY953_Data);
//		serial_port_1_TX(GY953_Data[1]);
//		serial_port_1_TX(GY953_Data[2]);
//		Wbn_get(Wbn,GY953_Data);
//		Acc_get(acc_b,GY953_Data);
//		acc_b[0]=((acc_b[0]-dev)/300)*300;
//		Acc_b[0]=acc_b[0];
//		GY953_Data[2]=acc_b[0];
//		GY953_Data[2]=acc_b[0]>>8;
//		Acc_b[1]=acc_b[1];
//		Acc_b[2]=acc_b[2];
//		m_multiply(Wbn,Acc_b,Acc_n,3,3,3,1);
//		vnx+=Acc_n[0]*0.005;
//		snx+=vnx*0.005;
//		if(i%200==0)
//		{
//			snx=((int)(snx*1000+0.5))/1000;
//			LCD_PrintoutFloat(20,5,snx);
//		}
		if(g_remote_frame_state)
		{
		//send_data2PC(3,ACC_TYPE,GY953_Data);
//		send_data2PC(3,MAG_TYPE, Data);
//		send_data2PC(3,ANGLE_TYPE,GY953_Data);
//		send_data2PC(3,FOUR_TYPE,GY953_Data);
		}
//		i++;
		delay_ms(20);
	}
	
}

