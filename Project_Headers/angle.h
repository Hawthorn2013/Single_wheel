#ifndef ANGLE_H_
#define ANGLE_H_


#ifdef __ANGLE_C_
struct
{

	SWORD PITCH_angle_zero;
	SWORD PITCH_anglespeed_zero;
	SWORD ROLL_angle_zero;
	SWORD ROLL_anglespeed_zero;
	SWORD YAW_angle_zero;
	SWORD YAW_anglespeed_zero;
}angle_data = { 1983, 974, 0, 0, 0, 0 };//{ 1922, 974, 0, 0, 0, 0 }

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





void set_PITCH_angle_zero(SWORD zero);
void set_PITCH_angle_speed_zero(SWORD zero);
void set_ROLL_angle_zero(SWORD zero);
void set_ROLL_angle_speed_zero(SWORD zero);
void set_YAW_angle_zero(SWORD zero);
void set_YAW_angle_speed_zero(SWORD zero);


#endif /* ANGLE_H_ */
