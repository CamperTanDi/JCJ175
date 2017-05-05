#include "define.h"



void DelayMS(unsigned int iMs) 
{ 
 	unsigned int i=0;
	unsigned int j=0; 
   	for(i=0;i<iMs;i++) 
	{
    	for(j=0;j<130;j++)
		 _Nop();
	}        
} 
void Ht1621_Init(void)
{
	HT1621_CSEN_OUT;
	HT1621_RDEN_OUT;
	HT1621_WREN_OUT;
	HT1621_DATEN_OUT;	                       
	HT1621_CS_HIGH;		
	DelayMS(200);
	HT1621_WR_LOW;
	DelayMS(200);
	HT1621_DAT_HIGH;
	DelayMS(200);
	Ht1621_WriteCommand(LCDBIAS134);
	Ht1621_WriteCommand(LCDRC256K);    
	Ht1621_WriteCommand(LCDSYSDIS);
	Ht1621_WriteCommand(LCDWDTDIS);
	Ht1621_WriteCommand(LCDSYSEN);
	Ht1621_WriteCommand(LCDTONE2K);
	Ht1621_CLR();
	Ht1621_WriteCommand(LCDON);
}
void Ht1621_WriteByte(unsigned char pdata,unsigned char cnt)
{
	unsigned char i;
	for (i=0;i<cnt;i++)
	{ 
	    HT1621_WR_LOW;
		_Nop(); 
		if(pdata&0x80)HT1621_DAT_HIGH;
		else HT1621_DAT_LOW;	      
	    _Nop(); 
	    HT1621_WR_HIGH;
	    _Nop(); 
	    pdata<<=1; 
	}		
}

void Ht1621_WriteCommand(unsigned char CommandWord)
{
	GIE=0;
	HT1621_CS_LOW;
	_Nop();
	Ht1621_WriteByte(0x80,0x04);	//Ð´ÃüÁîÍ·10000
	Ht1621_WriteByte(CommandWord,0x08);
	_Nop();
	HT1621_CS_HIGH;
	_Nop();
	HT1621_DAT_HIGH;
	GIE=1;
}
void Ht1621_CLR(void)
{
	unsigned char i;
	HT1621_CS_LOW;
	Ht1621_WriteByte(LCDWRITE,3);
	Ht1621_WriteByte(0,6);
	for(i=0;i<10;i++)
	{
		Ht1621_WriteByte(0,8);
	}
	HT1621_CS_HIGH;
}
void Ht1621_Disp(unsigned char *pdata)
{
	unsigned char i;
	HT1621_CS_LOW;
	Ht1621_WriteByte(LCDWRITE,3);
	Ht1621_WriteByte(0,6);
	for(i=0;i<10;i++)
	{
		Ht1621_WriteByte(*pdata++,8);
	}
	HT1621_CS_HIGH;
}
