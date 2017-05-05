#include "define.h"
bank2 volatile unsigned char RegCRC=0;
bank2 volatile bit SHT10_ErrorFlag=0;
//一个nop是6.5us,延时130us
void SHT_Delay1(void) 
{ 
	unsigned char i;
   	for(i=0;i<20;i++)
	 {_Nop();}
}
//延时65us
void SHT_Delay2(void) 
{ 
	unsigned char i;
   	for(i=0;i<10;i++)
	 {_Nop();}
}
unsigned char bitSwap(unsigned char sdata)
{
	unsigned char i;
	unsigned char returnchar=0;
	for(i=0;i<8;i++)
	{
		returnchar<<=1;
		if(sdata&0x01)returnchar|=0x01;
		sdata>>=1;
	}
	return returnchar;
}
void SHT10_Init(void)
{
	DelayMS(20);	
	SHT10_SCLEN_OUT;
	SHT10_SDAEN_OUT;	
	SHT10_SoftReset();
	DelayMS(20);	
}

void SHT10_Reset(void)
{
	unsigned char i; 
	SHT10_SDAEN_OUT;
	SHT10_SDA_HIGH;SHT_Delay1(); 
  	for(i=0;i<12;i++)
	{
		SHT10_SCL_HIGH;SHT_Delay1(); 
		SHT10_SCL_LOW;SHT_Delay1(); 
	}
	//Start时序
	SHT10_SDA_HIGH;SHT_Delay1(); 
	SHT10_SCL_HIGH;SHT_Delay1(); 
	SHT10_SDA_LOW;SHT_Delay1(); 
	SHT10_SCL_LOW;SHT_Delay1(); 
	SHT10_SCL_HIGH;SHT_Delay1(); 
	SHT10_SDA_HIGH;SHT_Delay1(); 
	SHT10_SCL_LOW;SHT_Delay1(); 
	SHT10_SDAEN_IN;
}
unsigned char SHT10_SoftReset(void)
{ 
  	unsigned char error=0;  
  	SHT10_Reset();              //SHT10_Reset communication
  	error+=SHT10_WriteByte(RESET);       //send RESET-command to sensor
	if(!error)RegCRC=0;  	
	return error;                     //error=1 in case of no response form the sensor
}

unsigned char SHT10_ReadByte(unsigned char ack)
{
	unsigned char bytetmp=0;
	char i;
	SHT10_SDA_HIGH;SHT_Delay1(); 
	SHT10_SDAEN_IN;
	for(i=0;i<8;i++)
	{				
		SHT10_SCL_HIGH;SHT_Delay1(); 
		bytetmp<<=1;
		if(SHT10_SDA)bytetmp|=1;SHT_Delay2(); 	
		SHT10_SCL_LOW;SHT_Delay1(); 		
	}	
	SHT10_SDAEN_OUT;
	SHT10_SDA=!ack;	SHT_Delay2(); 
	SHT10_SCL_HIGH;SHT_Delay1(); 	
	SHT10_SCL_LOW;SHT_Delay1(); 
	SHT10_SDA_HIGH;SHT_Delay1(); 
	return bytetmp;
}
unsigned char SHT10_WriteByte(unsigned char bytetmp)
{
	unsigned char i=0;
	unsigned char error=0;
	unsigned char i;
	SHT10_SDAEN_OUT;
	for(i=0;i<8;i++)
	{
		if(bytetmp&0x80)
		{SHT10_SDA_HIGH;SHT_Delay1();}
		else 
        {SHT10_SDA_LOW;SHT_Delay1(); }
		SHT10_SCL_HIGH;SHT_Delay1(); 
		SHT10_SCL_LOW;SHT_Delay1(); 
		bytetmp<<=1;
	}	
	SHT10_SDAEN_IN;
	SHT10_SCL_HIGH;SHT_Delay1(); 
	error=SHT10_SDA;SHT_Delay2(); 
	SHT10_SCL_LOW;SHT_Delay1(); 	
	SHT10_SDAEN_OUT;
	return error;
}
unsigned char ShiftBitCRC(unsigned char crc,unsigned char pdata)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if((crc&0x80)==(pdata&0x80))
		{
			crc<<=1;crc&=0xfe;
		}
		else
		{
			crc<<=1;crc|=0x01;crc^=0x30;
		}
		pdata<<=1;	
	}
	return crc;
}
unsigned char SHT10_CheckCRC(unsigned char Command,unsigned char *InBuf,unsigned char bytes,unsigned char it_crc)
{
	unsigned char me_crc;
	char i;
	me_crc=bitSwap(RegCRC&0x0f);
	me_crc=ShiftBitCRC(me_crc,Command);
	for(i=0;i<bytes;i++)
	{
		me_crc=ShiftBitCRC(me_crc,*(InBuf+i));
	}
	me_crc=bitSwap(me_crc);
	if(me_crc==it_crc)
	return 0;
	else 
	return 1;
}
unsigned char SHT10_ReadStatusReg(unsigned char *p_value)
{ 
	unsigned char it_crc;
  	unsigned char error=0;
  	SHT10_Reset();                  
  	error=SHT10_WriteByte(STATUSREAD);
  	*p_value=SHT10_ReadByte(ACK);       
  	it_crc=SHT10_ReadByte(NOACK);  	
	error+=SHT10_CheckCRC(STATUSREAD,p_value,1,it_crc);
	if(!error)RegCRC=*p_value;
  	return error;                    
}

unsigned char SHT10_WriteStatusReg(unsigned char p_value)
{ 
 	unsigned char error=0;
  	SHT10_Reset();                
  	error+=SHT10_WriteByte(STATUSWRITE);
  	error+=SHT10_WriteByte(p_value);   
	if(!error)RegCRC=p_value;
  	return error;                     
}

unsigned char SHT10_ReadValue(unsigned char Command,unsigned int *p_value)
{
	unsigned char it_crc;
	unsigned char error=0;
	unsigned char aa[2];
	aa[0]=SHT10_ReadByte(ACK);
	aa[1]=SHT10_ReadByte(ACK);	
	*p_value=aa[0]<<8|aa[1];
	it_crc=SHT10_ReadByte(NOACK);
	switch(Command)	
	{
		case HUMI:
			error=SHT10_CheckCRC(HEN,(unsigned char *)aa,2,it_crc);		
			break;
		case TEMP:
			error=SHT10_CheckCRC(TEN,(unsigned char *)aa,2,it_crc);
			break;
	}		
	return error;
}
unsigned char SHT10_ReadCommand(unsigned char Command)
{
	unsigned char error =0;
	SHT10_Reset();
	switch(Command)
	{
		case HUMI:
			error=SHT10_WriteByte(HEN);
			break;
		case TEMP:
			error=SHT10_WriteByte(TEN);
			break;
	}
	return error;
}


