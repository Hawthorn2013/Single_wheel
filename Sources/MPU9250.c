/*
 * init_MPU9250.c
 *
 *  Created on: Jul 10, 2015
 *      Author: Paul
 */

#include "includes.h"

void init_MPU9250(void)
{
	Reg_Write(PWR_MGMT_1,0x80);		//reset
	Reg_Write(PWR_MGMT_1,0x01);		//select clock
	Reg_Write(PWR_MGMT_2,0x00);		//enable Acc&Gyro
	Reg_Write(CONFIG,0x06);			//disable FSYNC;
	Reg_Write(GYRO_CONFIG,0x00);	//+250dps	DLPF bandwidth 3600, delay 0.17 Fs 8kHz
	Reg_Write(ACCEL_CONFIG,0x00);	//¡À2g
	Reg_Write(ACCEL_CONFIG2,0x00);	//Acc DLPF
	Reg_Write(SMPLRT_DIV,0x07);
	//Reg_Write(FIFO_EN,0x78);		//enable gyro&Acc FIFO
	Reg_Write(USER_CTRL,0x10);
	
}

int Reg_Read(uint8_t reg,uint16_t* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint16_t tmp_rx;
	
	tmp_tx = 0x88040000|reg|0x80;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint16_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;

	tmp_tx = 0x08040000|0xff;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint16_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	*Data=tmp_rx;
	return 1;
}

int Reg_Write(uint8_t reg,uint16_t Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint16_t tmp_rx;
	
	tmp_tx = 0x88040000|(reg&0x7f);
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint16_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = 0x08040000|Data;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint16_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return 1;
}
