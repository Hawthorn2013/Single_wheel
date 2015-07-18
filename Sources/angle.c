#define __ANGLE_C_
#include "includes.h"

unsigned int jos[4]={0};
unsigned int ADResult[4]={0};
unsigned int AngleResult[4]={0}; //ǰ��Ƕ�
float AngleCalculate[4]={0};
unsigned int AngleResult_balance[4]={0}; //���ҽǶ�
float AngleCalculate_balance[4]={0};

/*-----------------------------------------------------------------------*/
/*-----------��������ģ���ֵ����-------------*/
/*--------------�ǶȽ��ٶ�-----------------*/
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
/*-----------��������ģ�����ݴ���-------------*/
/*-----------�����Ǿ���0.67-----------------*/
/*-----------------------------------------------------------------------*/
void angle_calculate(void)
{
	AngleCalculate[0]=((float)AngleResult[0]-1936)/5.49; //�����ǵĽǶ�  ǰ��ĽǶ�//1380   
	AngleCalculate[1]=((float)AngleResult[1]-976)/0.67;//�����ǽ��ٶ�
//	AngleCalculate[2]=((float)AngleResult[2]-1532)/5; // ת��Ƕ�
//	AngleCalculate[3]=((float)AngleResult[3]-1378)/0.67; //���ٶ�z��  //1380
}

/*-----ǰ�����������ݴ���---*/
void angle_calculate_balance(void)
{
	AngleCalculate_balance[0]=((float)AngleResult_balance[0]-1936)/5.49; //�����ǵĽǶ�  ǰ��ĽǶ�//1380   
	AngleCalculate_balance[1]=((float)AngleResult_balance[1]-976)/0.67;//�����ǽ��ٶ�
}


