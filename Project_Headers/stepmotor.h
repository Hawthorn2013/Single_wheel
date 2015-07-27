#ifndef STEPMOTOR_H_
#define STEPMOTOR_H_


#define SMotor_A SIU.GPDO[16].B.PDO	//PB0
#define SMotor_B SIU.GPDO[17].B.PDO	//PB1
#define SMotor_C SIU.GPDO[72].B.PDO	//PE8
#define SMotor_D SIU.GPDO[73].B.PDO	//PE9

extern BYTE g_Control;
extern int stepspeed;
extern int timer;
extern int ABCD;

void Pit_1ms(void);
void init_Stepmotor(void);
void Motor_stop(void);
void Motor_A(void);
void Motor_B(void);
void Motor_C(void);
void Motor_D(void);
void stepmotor_rotate(void);
void stepmotor_video(int num);
void stepmotor_balance(void);


#endif /* STEPMOTOR_H_ */
