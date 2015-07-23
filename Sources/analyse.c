/*
 * analyse.c
 *
 *  Created on: Jul 13, 2015
 *      Author: Paul
 */
#include "includes.h"
#define g (9.8)
#define stime (0.001)		//����ʱ�� ��λs

int32_t MPU_Data[6];	//�����ǣ����ٶȼ�����

int32_t xdev;	//���ƫ���ֵ
int32_t ydev;	//���ƫ���ֵ
int32_t zdev;	//���ƫ���ֵ
int32_t xA=400;		//���ƫ���ֵ
float xa=0;		//x�᷽����ٶ�
float xv=0;		//y�᷽����ٶ�
float xs=0;		//z�᷽����ٶ�

void distance(void)
{
	int32_t temp;
	const float range=65536;
	Read_GYRO_ACC(MPU_Data);	//��ȡ����������
	
	temp=(MPU_Data[3]-xdev)/xA;
	xa=-temp*xA*4*g;	//��x������ٶ��˲�
	xa=xa/range;

	xv=xv+xa*stime;			//��x�����ٶ�
	xs=xs+xv*stime;			//��x�������

	if(xs>=2)				//2�ױ�־
		D1=0;
	
	//��ʾ����ʾ
	LCD_Write_Num(80,1,MPU_Data[3],5);
//	LCD_Write_Num(45,2,MPU_Data[4],5);
//	LCD_Write_Num(45,3,MPU_Data[5],5);
	LCD_PrintoutFloat(45, 2, xa);
	LCD_PrintoutFloat(45, 4, xv);
	LCD_PrintoutFloat(45, 6, xs);
	
	delay_ms(1);//ÿ5ms����һ�Σ���stime��Ӧ  ���᲻��������
}

void filter_display(void)
{
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
	MPU_Data[3]=temp1;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(i++);
	serial_port_1_TX(0xAA);
	
	temp=(MPU_Data[3]-xdev)/xA;
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

void acc_display(uint32_t* i)	//iΪѭ������,�˲���
{
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
	MPU_Data[3]=temp1;
	
	temp=(MPU_Data[3]-xdev)/xA;
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
	MPU_Data[4]=temp1;
	
	temp=(MPU_Data[4]-ydev)/xA;
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
	MPU_Data[5]=temp1;
	
	temp=(MPU_Data[5]-zdev)/xA;
	xa_1=temp*xA+zdev;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii++);
	serial_port_1_TX(0xCC);
	
	sqrt(9);
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

//--------------------------------------------------------
//���ܣ����������������΢�ַ��� dq/dt=0.5*W*q
//��ڲ�������Ԫ����ֵ(����),����T(Ϊ�������ڵ���ʮ��֮һ,��λus),���ٶȾ���W,�Լ������Ԫ��
//����ֵ��q1,���������Ԫ��
//--------------------------------------------------------
void Runge_Kutta(float q0[],float T,float W[],float q1[])
{
	float k1[4],k2[4],k3[4],k4[4],kt[4],ktt[4],y1;
	float q2[4],q3[4];
	float mat[4];
	float b;//��һ��ϵ��
	int i;
	for(i=0;i<40;i++)
	{
		m_multiply(W,q0,mat,4,4,4,1);
		m_scalar(mat,0.5,k1,4,1);	//��k1
		
		m_scalar(k1,T/2,kt,4,1);
		m_add(kt,q0,ktt,4,1);
		m_multiply(W,ktt,k2,4,4,4,1);//��k2
		
		m_scalar(k2,T/2,kt,4,1);
		m_add(kt,q0,ktt,4,1);
		m_multiply(W,ktt,k3,4,4,4,1);//��k3
		
		m_scalar(k3,T,kt,4,1);
		m_add(kt,q0,ktt,4,1);
		m_multiply(W,ktt,k4,4,4,4,1);//��k4
		
		m_scalar(k2,2,ktt,4,1);
		m_add(ktt,k1,kt,4,1);
		m_scalar(k3,2,ktt,4,1);
		m_add(kt,ktt,kt,4,1);
		m_add(kt,k4,kt,4,1);
		m_scalar(kt,T/6,kt,4,1);
		m_add(kt,q0,q1,4,1);		//���q1
		
		q0=q1;
		delay_us(T);
	}
	
	//��һ��
	b=1/sqrt(q1[0]*q1[0]+q1[1]*q1[1]+q1[2]*q1[2]+q1[3]*q1[3]);
	m_scalar(q1,b,q1,4,1);
}

void W_get(float W[])
{
	int32_t wx,wy,wz;
	wx=MPU_Data[0];
	wy=MPU_Data[1];
	wz=MPU_Data[2];
	W[0]=0;
	W[1]=-wx;
	W[2]=-wy;
	W[3]=-wz;
	W[4]=wx;
	W[5]=0;
	W[6]=wz;
	W[7]=-wy;
	W[8]=wy;
	W[9]=-wz;
	W[10]=0;
	W[11]=wx;
	W[12]=wz;
	W[13]=wy;
	W[14]=-wx;
	W[15]=0;
	
}

void angle_get(float q[],float sita,float gama,float fai)
{
	float T12,T22,T31,T32,T33;
	T12=2*(q[1]*q[2]+q[0]*q[3]);
	T22=q[0]*q[0]-q[1]*q[1]+q[2]*q[2]-q[3]*q[3];
	T32=2*(q[2]*q[3]-q[0]*q[1]);
	T33=q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3];
	sita=asin(T32);
	gama=atan(-T31/T33);
	fai=atan(T12/T22);
}

float* Cb_n(float q[])
{
	float T11,T12,T13,T21,T22,T23,T31,T32,T33;
	float Cbn[9];
	T11=q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3];
	T12=2*(q[1]*q[2]+q[0]*q[3]);
	T13=2*(q[1]*q[3]-q[0]*q[2]);
	T21=2*(q[1]*q[2]-q[0]*q[3]);
	T22=q[0]*q[0]-q[1]*q[1]+q[2]*q[2]-q[3]*q[3];
	T23=2*(q[2]*q[3]+q[0]*q[1]);
	T31=2*(q[1]*q[3]+q[0]*q[2]);
	T32=2*(q[2]*q[3]-q[0]*q[1]);
	T33=q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3];
	Cbn[0]=T11;
	Cbn[1]=T12;
	Cbn[2]=T13;
	Cbn[3]=T21;
	Cbn[4]=T22;
	Cbn[5]=T23;
	Cbn[6]=T31;
	Cbn[7]=T32;
	Cbn[8]=T33;
	return Cbn;
}


//float abs(float data)
//{
//	if (data<0)
//		data=0-data;
//	return data;
//}
