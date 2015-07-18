#ifndef ANGLE_H_
#define ANGLE_H_

extern unsigned int AngleResult[4];
extern float AngleCalculate[4];
extern unsigned int AngleResult_balance[4];
extern float AngleCalculate_balance[4];


void angle_read(unsigned int *AngleGra);
void angle_calculate(void);
void angle_calculate_balance(void);

#endif /* ANGLE_H_ */
