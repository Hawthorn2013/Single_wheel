#ifndef ANGLE_H_
#define ANGLE_H_


#ifdef __ANGLE_C_
struct
{
	float PITCH_angle_zero;
	float PITCH_anglespeed_zero;
	float ROLL_angle_zero;
	float ROLL_anglespeed_zero;
	float YAW_angle_zero;
	float YAW_anglespeed_zero;
}angle_data = { 1936, 976, 0, 0, 0, 0 };
#else
extern struct
{
	float PITCH_angle_zero;
	float PITCH_anglespeed_zero;
	float ROLL_angle_zero;
	float ROLL_anglespeed_zero;
	float YAW_angle_zero;
	float YAW_anglespeed_zero;
}angle_data;
#endif

extern unsigned int AngleResult[4];
extern float AngleCalculate[4];

void angle_read(unsigned int *AngleGra);
void angle_calculate(void);
void set_PITCH_angle_zero(DWORD zero);
void set_PITCH_angle_speed_zero(DWORD zero);
void set_ROLL_angle_zero(DWORD zero);
void set_ROLL_angle_speed_zero(DWORD zero);
void set_YAW_angle_zero(DWORD zero);
void set_YAW_angle_speed_zero(DWORD zero);

#endif /* ANGLE_H_ */
