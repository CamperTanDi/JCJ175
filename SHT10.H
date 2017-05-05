#ifndef _SHT10_
#define _SHT10_

#define SHT10_SDA 		RC4
#define SHT10_SCL 		RC3
#define SHT10_SDAEN 	TRISC4
#define SHT10_SCLEN 	TRISC3

#define SHT10_SDA_HIGH	SHT10_SDA=1
#define SHT10_SDA_LOW 	SHT10_SDA=0
#define SHT10_SCL_HIGH	SHT10_SCL=1
#define SHT10_SCL_LOW 	SHT10_SCL=0	

#define SHT10_SDAEN_OUT	SHT10_SDAEN=0
#define SHT10_SDAEN_IN	SHT10_SDAEN=1
#define SHT10_SCLEN_OUT	SHT10_SCLEN=0
#define SHT10_SCLEN_IN	SHT10_SCLEN=1

#define TEN 			0x03
#define HEN 			0x05
#define STATUSREAD 		0x07
#define STATUSWRITE 	0x06
#define STATUSHOTEN 	0x04
#define STATUSHOTDIS 	0x00
#define RESET 			0x1e 
#define NOACK 			0
#define ACK   			1
enum {TEMP,HUMI};

extern void SHT10_Start(void);
extern unsigned char SHT10_ReadByte(unsigned char ack);
extern unsigned char SHT10_WriteByte(unsigned char bytetmp);
extern unsigned char SHT10_ReadStatusReg(unsigned char *p_value);
extern unsigned char SHT10_WriteStatusReg(unsigned char p_value);
extern unsigned char SHT10_SoftReset(void);
extern unsigned char SHT10_ReadCommand(unsigned char Command);
extern unsigned char SHT10_ReadValue(unsigned char Command,unsigned int *p_value);
extern void SHT10_Reset(void);
extern void SHT10_Init(void);
extern bank2 volatile unsigned char RegCRC;
extern bank2 volatile bit SHT10_ErrorFlag;
#define INITLOSTMAX	5
#endif