
#ifndef GY953_H_
#define GY953_H_

#define SET_A 0x01//ÅäÖÃ¼Ä´æÆ÷A
#define CONTROL_B 0x02//¿ØÖÆ¼Ä´æÆ÷B
#define STATE_D 0x23//×´Ì¬¼Ä´æÆ÷D
#define STATE_C 0x24//Èı¼Æ¾«¶È×´Ì¬¼Ä´æÆ÷ 
#define ACC_X_H 0x03
#define ACC_X_L 0x04





void init_GY953(void);
int GY953_Read(uint8_t reg,uint8_t* Data);
int GY953_multi_Read(uint8_t* Data);
int GY953_Write(uint8_t reg,uint8_t Data);
int Read_Precision(BYTE* Data);	
int Read_GYalldata(BYTE* Data);






#endif /* GY953_H_ */
