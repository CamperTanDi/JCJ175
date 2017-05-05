#ifndef _I2_RW_
#define _I2_RW_

extern void I2_Init(void);	
extern void I2_Read(unsigned char addr,unsigned char *pOutBuf,unsigned char read_bytes);   
extern void I2_Write(unsigned char addr,unsigned char *pInBuf,unsigned char write_bytes);	

#endif