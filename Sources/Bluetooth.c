#define __BLUETOOTH_C_
#include "includes.h"

uint8_t GY953_Data[41];
int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
int g_start_PCcontrol=0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH];
BYTE remote_frame_data_send[REMOTE_FRAME_LENGTH];
//BYTE g_device_NO = WIFI_ADDRESS_CAR_2;	/* �豸�� ��WiFi��ַ */
SWORD ans=0;


/*-----------------------------------------------------------------------*/
/* ִ��Զ������                                                          */
/*-----------------------------------------------------------------------*/
void execute_remote_cmd(const BYTE *data)
{
	BYTE cmd = 0;
	
	cmd = (BYTE)(data[0]);
	switch (cmd)
	{
		
		/* ��ʼ��λ������ */
		case CMD_START_CONTROL:
			g_start_PCcontrol=1;
		break;
		
		/* ��λ�����ƽ��� */
		case CMD_STOP_CONTROL:
			g_start_PCcontrol=0;
		break;
		
		/* ���������Ǳ궨���� */
		case CMD_SET_PITCH_ANGLE_ZERO :
			set_PITCH_angle_zero(*((SWORD *)(&(data[2]))));
			D7=~D7;
		break;
		case CMD_SET_PITCH_ANGLE_SPEED_ZERO :
			set_PITCH_angle_speed_zero(*((SWORD *)(&(data[2]))));
		break;
		
		case CMD_SET_ANGLE_KP :
			set_angle_KP(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ANGLE_KI :
			set_angle_KI(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ANGLE_KD :
			set_angle_KD(*((SWORD *)(&(data[2]))));
		break;
		
		/* ��������Ǳ궨���� */
		case CMD_SET_ROLL_ANGLE_ZERO :
			set_ROLL_angle_zero(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ROLL_ANGLE_SPEED_ZERO :
			set_ROLL_angle_speed_zero(*((SWORD *)(&(data[2]))));
		break;
		
		/* ����������Ǳ궨���� */
		case CMD_SET_YAW_ANGLE_ZERO :
			set_YAW_angle_zero(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_YAW_ANGLE_SPEED_ZERO :
			set_YAW_angle_speed_zero(*((SWORD *)(&(data[2]))));
			D7=~D7;
		break;
		
		
		/* �����ֵ������ */
		case CMD_SET_MOTOR1_TARGET :
			set_speed_target(*((SWORD *)(&(data[2]))));
		break;
		case CMD_STOP_SPEED :
			set_speed_target((SWORD)0);
		break;
		case CMD_SET_MOTOR1_PWM_TARGET:
			set_pwm1_target(*((SWORD *)(&(data[2]))));
		break;
		
		case CMD_SET_MOTOR1_KP :
			set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_MOTOR1_KI :
			set_speed_KI(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_MOTOR1_KD :
			set_speed_KD(*((SWORD *)(&(data[2]))));
		break;

		
		
		/* ����ƽ�������� 	*/
		case CMD_SET_MOTOR2_PWM_TARGET :
			set_pwm2_target(*((SWORD *)(&(data[2]))));
		break;
		
		/* ����ǵ������ 	*/
		case CMD_SET_MOTOR3_PWM_TARGET :
			set_pwm3_target(*((SWORD *)(&(data[2]))));
		break;
		
#if 0		
		/* ����   ����ͨ��	*/
		case CMD_NET :
		g_net_control_data.is_new_cmd = 1;
		g_net_control_data.cmd = *((WORD *)(&(data[2])));
		break;
#endif		

	}
}


/*-----------------------------------------------------------------------*/
/* ����Զ������֡                                                        */
/* �ڶ���                                                                                              */
/* 5A ��������(1B) У����(1B)              */
/* δ�ı�ԭ����֡�Ĵ󲿷ֶ��弰����                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame_2(BYTE rev)
{
	uint8_t sum;
	uint8_t num[4]={0};
	if (g_remote_frame_cnt == 0)
	{
		if(rev==0xA5)
		{
			remote_frame_data[0]=rev;
			g_remote_frame_cnt++;
		}
	}
	else if(g_remote_frame_cnt==1)
	{
		remote_frame_data[1]=rev;
		g_remote_frame_cnt++;
	}
	else if(g_remote_frame_cnt==2)
	{
		remote_frame_data[2]=rev;
		g_remote_frame_cnt=0;
//		LCD_Write_Num(80,1,remote_frame_data[0],5);
//		LCD_Write_Num(80,2,remote_frame_data[1],5);
//		LCD_Write_Num(80,3,remote_frame_data[2],5);
		sum=(uint8_t)(remote_frame_data[0]+remote_frame_data[1]);
		if(sum==remote_frame_data[2])
		{
			if(remote_frame_data[1]==0x57)//����У׼
				{
					GY953_Write(0x02,0x15);
					g_remote_frame_state=REMOTE_FRAME_STATE_OK;
				}
			else if(remote_frame_data[1]==0x58)//�����ƽ�׼
			{
				D2=~D2;
				GY953_Write(0x02,0x19);
			}
			else if(remote_frame_data[1]==0x75)//���ᴫ��������
			{
				Read_Precision(num);
				generate_remote_frame_2(PREC_TYPE,4,num);
//				send_data2PC(3,PREC_TYPE,GY953_Data);
			}
			D0=~D0;
		}
	}
	
	return 1;
	

//	uint8_t sum;
//	if (g_remote_frame_cnt == 0)	//������ʼλ
//	{
//		if (rev == 0x5A)
//		{
//			D0=~D0;
//			remote_frame_data[g_remote_frame_cnt++] = 0x5A;
//		}
//	}
//	else if (g_remote_frame_cnt == 1)	//������������λ
//	{
//		if (rev == 0x57)	//����У׼
//		{
//			D1=~D1;
//			remote_frame_data[g_remote_frame_cnt++] = 0x57;
//		}
////		else if(rev==0x58)	//����У׼
////		{
////			D2=~D2;
////			remote_frame_data[g_remote_frame_cnt++] = 0x58;
////		}
////		else if(rev==0x85)//��ȡ����
////		{
////			D3=~D3;
////			remote_frame_data[g_remote_frame_cnt++] = 0x85;
////		}
////		else if(rev==0x75)//���ȣ�Ƶ��
////		{
////			remote_frame_data[g_remote_frame_cnt++] = 0x75;
////		}
//		else
//		{
//			g_remote_frame_cnt=0;
//		}
//	}
//	else if (g_remote_frame_cnt == 2)	//����У��λ
//	{
//		sum=(uint8_t)(remote_frame_data[0]+remote_frame_data[1]);
//		if(rev==sum)
//		{
//			remote_frame_data[g_remote_frame_cnt] = rev;
//			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
//			if(remote_frame_data[1]==0x57)
//			{
//				GY953_Write(0x02,0x13);
//			}
////			else if(remote_frame_data[1]==0x58)	
////			{
////				send_data2PC(3, PREC_TYPE, Data);
////				GY953_Write(0x02,0x13);
////			}
////			else if(remote_frame_data[1]==0x75)
////			{
////				Read_Precision(Data);
////				send_data2PC(3, PREC_TYPE, Data);
////			}
//		}
//		g_remote_frame_cnt=0;
//	}
//	
//
//	return g_remote_frame_state;
}


/*-----------------------------------------------------------------------*/
/* ���ɲ�������������λ��                                                    */
/* 5A 5A ��������(1B) ����(1B) ������   У���(1B)	*/
/* ���� :   type ������������                                                 */
/*        length data����                                                */
/*        data������������                                   */
/*		     У��λ��������ȫ��λ�ĺ�У��   */
/*-----------------------------------------------------------------------*/
void generate_remote_frame_2(BYTE type, BYTE length, const BYTE data[])
{
	WORD i = 0, j = 0;
	D7=~D7;
	remote_frame_data_send[i++] = 0x5A;
	remote_frame_data_send[i++] = 0x5A;
	remote_frame_data_send[i++] = type;
	remote_frame_data_send[i++] = length;
	for (j = 0; j < length; j++)
	{
		remote_frame_data_send[i++] = data[j];
	}
	remote_frame_data_send[i] = check_sum(remote_frame_data_send, length+4);
	
	serial_port_1_TX_array(remote_frame_data_send, length+5);
}


/*-----------------------------------------------------------------------*/
/* ����GY953������PC��λ��                                                          */
/* ����generate_remote_frame_2()                                          */
/* sensor����������
/* datatype��������
/*-----------------------------------------------------------------------*/
void send_data2PC(BYTE sensor, BYTE type, BYTE data[])
{
	if(sensor==ENC03)
	{D6=~D6;
		if(type==GYR_TYPE)
			generate_remote_frame_2( type, 2, (const BYTE *)(&data[1]));
		else if(type==ANGLE_TYPE)
			generate_remote_frame_2( type, 2, (const BYTE *)(&data[0]));
	}
	else if(sensor==MPU9250)
	{
		if(type==ACC_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[6]));
		else if(type==GYR_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[0]));
	}
	else if(sensor==GY953)
		{
			if(type==ACC_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[2]));
			else if(type==GYR_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[8]));
			else if(type==ANGLE_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[20]));
			else if(type==MAG_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[14]));
			else if(type==FOUR_TYPE)
				generate_remote_frame_2( type, 8, (const BYTE *)(&data[26]));
			else if(type==PREC_TYPE)
				generate_remote_frame_2(type,1, (const BYTE *)(&data[35]));
		}
	
}


/*-----------------------------------------------------------------------*/
/* ���У��                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_xor(const BYTE *data, BYTE length)
{
	int i;
	BYTE res = 0;
	
	for (i=0; i<length; i++)
	{
		res ^= data[i];
	}
	
	return res;
}

/*-----------------------------------------------------------------------*/
/* �ۼӺ�У��                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_sum(const BYTE *data, BYTE length)
{
	int i;
	WORD res = 0;
	
	for (i=0; i<length; i++)
	{
		res += data[i];
		res  = res&0x00FF;
	}
	
	return (BYTE)res;
}



