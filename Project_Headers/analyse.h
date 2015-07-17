/*
 * analyse.h
 *
 *  Created on: Jul 13, 2015
 *      Author: Paul
 */

#ifndef ANALYSE_H_
#define ANALYSE_H_

extern int32_t xdev;
extern int32_t ydev;
extern int32_t zdev;
extern int32_t xA;
extern int32_t Data[];
extern float xa;
extern float xv;
extern float xs;


extern void distance(void);
extern void filter_display(void);
extern void acc_display(uint32_t* i);
extern void temp_analyse(uint32_t* i);

#endif /* ANALYSE_H_ */
