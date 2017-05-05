#include "define.h"

bank3 volatile _keyInfo keyInfo; 
bit  Key_GetKey(void); 
void Key_Init(void);
bank1 volatile unsigned int  KeyCount[4]={0,0,0,0};
bank3 volatile unsigned char keyflag=0;
bank3 volatile unsigned char flagtimes=0;
bank3 volatile unsigned char keyflagtmer=0;
//const unsigned char keyIndex[]={1,0xff,0xff,0xff,2,3,0,0xff};
bank3 volatile unsigned char keyon=0; //判断是否有按键按下
void Key_Init(void)
{
	keyInfo.keyValue=0;
	keyflag=0;
	flagtimes=0;
	keyflagtmer=0;
	WPUB|=0x0e;
	OPTION&=0x7f;//开上拉电阻
	KEY_SETEN_IN;
	KEY_ADDEN_IN;
	KEY_DELEN_IN;
    	keyon=0;
}
/*************************************************************************************************************
函数名:Key_GetKey
功能:取键值
参数:无
返回:bit  =1有键按下 =0无键按下
*************************************************************************************************************/
bit Key_GetKey(void)
{
	unsigned char i=0,j=1,k=16;	
	unsigned int temp=0;
	unsigned char tmp=0;	
	
	for(i=0;i<4;i++)
	{
		if(KeyCount[i]>0){tmp++;keyInfo.keyValue=KeyCount[i];}		
	}
	if(tmp!=1)
	{
		for(i=0;i<4;i++)
		{
	//		T0IE=0;
			KeyCount[i]=0;
	//		T0IE=1;
		}
		flagtimes=0;
		keyflag=0;
		return 0;
	}
	
	for(i=0;i<4;i++)
	{			
		if((KeyCount[i]>=MS_20)&&(KeyCount[i]<MS_500))
		{
			if(!(keyInfo.keyStatus & j))
			{
				keyInfo.keyStatus|=j;
				temp|=j;
				break;				
			}
		} 
		else
		{
			keyInfo.keyStatus&=~j;
		}	
		if(KeyCount[i]>=MS_500)
		{
			if(!(keyInfo.keyStatus & k))
			{
				keyInfo.keyStatus|=k;
				temp|=k;	
				break;
			}
			else
			{
				temp=j;
				temp<<=8;
			}
		} 
		else
		{
			keyInfo.keyStatus&=~k;
		}
		j*=2;
		k*=2;		
	}

	keyInfo.keyValue=temp;
    keyon=1;
	return 1;	
}

