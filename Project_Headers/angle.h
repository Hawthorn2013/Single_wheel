#ifndef ANGLE_H_
#define ANGLE_H_


#ifdef __ANGLE_C_
struct
{
<<<<<<< HEAD
	float PITCH_angle_zero;
	float PITCH_anglespeed_zero;
	float ROLL_angle_zero;
	float ROLL_anglespeed_zero;
	float YAW_angle_zero;
	float YAW_anglespeed_zero;
}angle_data = { 1936, 976, 0, 0, 0, 0 };  //½Ç¶È ½ÇËÙ¶È
=======
	SWORD PITCH_angle_zero;
	SWORD PITCH_anglespeed_zero;
	SWORD ROLL_angle_zero;
	SWORD ROLL_anglespeed_zero;
	SWORD YAW_angle_zero;
	SWORD YAW_anglespeed_zero;
}angle_data = { 1983, 974, 0, 0, 0, 0 };//{ 1922, 974, 0, 0, 0, 0 }
>>>>>>> 8c625fc5adef5b34f082c5062cb56d0e19c3d5c6
#else
extern struct
{
	SWORD PITCH_angle_zero;
	SWORD PITCH_anglespeed_zero;
	SWORD ROLL_angle_zero;
	SWORD ROLL_anglespeed_zero;
	SWORD YAW_angle_zero;
	SWORD YAW_anglespeed_zero;
}angle_data;
#endif

extern unsigned int AngleResult[4];
extern float AngleCalculate[4];
extern unsigned int AngleResult_balance[4];
extern float AngleCalculate_balance[4];


void angle_read(unsigned int *AngleGra);
void angle_calculate(void);
<<<<<<< HEAD

void angle_calculate_balance(void);

void set_PITCH_angle_zero(DWORD zero);
void set_PITCH_angle_speed_zero(DWORD zero);
void set_ROLL_angle_zero(DWORD zero);
void set_ROLL_angle_speed_zero(DWORD zero);
void set_YAW_angle_zero(DWORD zero);
void set_YAW_angle_speed_zero(DWORD zero);
=======
void set_PITCH_angle_zero(SWORD zero);
void set_PITCH_angle_speed_zero(SWORD zero);
void set_ROLL_angle_zero(SWORD zero);
void set_ROLL_angle_speed_zero(SWORD zero);
void set_YAW_angle_zero(SWORD zero);
void set_YAW_angle_speed_zero(SWORD zero);
>>>>>>> 8c625fc5adef5b34f082c5062cb56d0e19c3d5c6

#endif /* ANGLE_H_ */
