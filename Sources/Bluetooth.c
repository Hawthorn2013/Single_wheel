#define __BLUETOOTH_C_
#include "includes.h"


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
	WORD cmd = 0;
	
	cmd = ((WORD)(data[0])<<8) | ((WORD)(data[1]));
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
			set_PITCH_angle_zero(*((DWORD *)(&(data[2]))));
		break;
		case CMD_SET_PITCH_ANGLE_SPEED_ZERO :
			set_PITCH_angle_speed_zero(*((DWORD *)(&(data[2]))));
		break;
		
		/* ��������Ǳ궨���� */
		case CMD_SET_ROLL_ANGLE_ZERO :
			set_ROLL_angle_zero(*((DWORD *)(&(data[2]))));
		break;
		case CMD_SET_ROLL_ANGLE_SPEED_ZERO :
			set_ROLL_angle_speed_zero(*((DWORD *)(&(data[2]))));
		break;
		
		/* ����������Ǳ궨���� */
		case CMD_SET_YAW_ANGLE_ZERO :
			set_YAW_angle_zero(*((DWORD *)(&(data[2]))));
		break;
		case CMD_SET_YAW_ANGLE_SPEED_ZERO :
			set_YAW_angle_speed_zero(*((DWORD *)(&(data[2]))));
		break;
		
		
		/* �����ֵ������ */
		case CMD_SET_MOTOR1_TARGET :
			set_speed_target(*((SWORD *)(&(data[2]))));
		break;
		case CMD_STOP_SPEED :
			set_speed_target((SWORD)0);
		break;
		case CMD_SET_MOTOR1_KP :
			set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_MOTOR1_KI :
			set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_MOTOR1_KD :
			set_speed_KP(*((SWORD *)(&(data[2]))));
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
/* �ڶ���                                                                */
/* �޸ĵ�ַλ����:Դ��ַ(1B)+Ŀ�ĵ�ַ(1B)                                */
/* 5A 5A ��������(1B) ����(1B) ������ У����(1B)              */
/* δ�ı�ԭ����֡�Ĵ󲿷ֶ��弰����                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame_2(BYTE rev)
{
	if (g_remote_frame_cnt == 0)	//����֡ͷ
	{
		if (rev == 0x5A)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0x5A;
		}
	}
	else if (g_remote_frame_cnt == 1)	//����֡ͷ
	{
		if (rev == 0x5A)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0x5A;
		}
		else
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt == 2)	//������������
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt == 3)	//���ճ���
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev+5>REMOTE_FRAME_LENGTH)	//�ж��Ƿ�ᵼ�»��������
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt>3 && g_remote_frame_cnt<=remote_frame_data[3]+3)	//����������
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt==remote_frame_data[3]+4)	//����У���ֽ�	
	{
		BYTE sum;
		remote_frame_data[g_remote_frame_cnt++] = rev;
		sum = check_sum((const BYTE *)(remote_frame_data), (BYTE)(remote_frame_data[3]+4));
		if (sum != remote_frame_data[remote_frame_data[3]+4])
		{
			g_remote_frame_cnt = 0;	//CheckSum Fail
		}
		else
		{
			g_remote_frame_cnt = 0;
			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
		}
	}
	
	return g_remote_frame_state;
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
/* ��������ΪGY953 alldata
/*-----------------------------------------------------------------------*/
void send_data2PC(BYTE type, BYTE data[])
{
	if(type==ACC_TYPE)
		generate_remote_frame_2( type, 6, (const BYTE *)(&data[2]));
	else if(type==GYR_TYPE)
		generate_remote_frame_2( type, 6, (const BYTE *)(&data[8]));
	else if(type==ANGLE_TYPE)
		generate_remote_frame_2( type, 6, (const BYTE *)(&data[20]));
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
		res  = (BYTE)res;
	}
	
	return (BYTE)res;
}



