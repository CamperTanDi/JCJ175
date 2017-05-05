#include "define.h"


void I2_Init(void)
{
	//EEPGD=0;//Ñ¡ÔñÊý¾Ý´æ´¢Æ÷	
}
void I2_Read(unsigned char addr,unsigned char *pOutBuf,unsigned char read_bytes)
{
	unsigned char i;
	for(i=0;i<read_bytes;i++)
	{
		*(pOutBuf+i)=eeprom_read(addr);
		addr++;
	}
}
void I2_Write(unsigned char addr,unsigned char *pInBuf,unsigned char write_bytes)
{
	unsigned char i;
	for(i=0;i<write_bytes;i++)
	{	
		eeprom_write(addr,*(pInBuf+i));
		addr++;
		while(WR);
	}
}
	

