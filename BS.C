#include "define.h"
bank2 volatile _bsInfo bsInfo;
bank2 volatile unsigned char bsvalueH[2]={0,0};
bank2 volatile unsigned char bsvalueL[2]={0,0};
bank2 volatile unsigned char bsindex=0;
void BS_Init(void);
void BS_Handle(void);
void BS_Init(void)
{
	TRISC1=1;
	TRISC2=1;//预置为输入模式
	PR2=0xff;//设置周期  
	CCP1CON=0x0c;//设置为PWM模式
	CCP2CON=0x0c;
	CCPR2L=0;
	CCPR1L=0;
	GIE=0;
	PEIE=1;
	T2CON=0x0;//无分频
	TMR2ON=1;
	TRISC1=0;
	TRISC2=0;//设置为输出模式
}


void BS_Handle(void)
{
	unsigned char i=0;
	long bstmp=0;
	long inttmp=0;	
	if(configItem.jzflag==0 && menuInfo.parType!=parbiansongindex)
	{
		for(i=0;i<CH_MAX;i++)
		{		
			if(configItem.outflag == 1)
			{
				inttmp=parItem[i].bslvalue;
			}
			else	
			{
				inttmp=parItem[i].bslvalue*5-parItem[i].bshvalue;		
				inttmp/=4;	
			}		
			if(adInfo.pv[i]<=parItem[i].bsl)
			{
				bstmp=inttmp;					
			}
			else if(adInfo.pv[i]>=parItem[i].bsh)
			{
				bstmp=parItem[i].bshvalue;
			}
			else
			{
				bstmp=adInfo.pv[i]-parItem[i].bsl;				
				bstmp*=(parItem[i].bshvalue-inttmp);
				bstmp/=(parItem[i].bsh-parItem[i].bsl);
				bstmp+=inttmp;
			}
			if(bstmp-1<0)bstmp=1;					
			bsInfo.pv[i]=(int)bstmp;			
		}	
	}
	else
	{
		switch(configItem.jzflag)
		{
			case 1:
				bsInfo.pv[0]=parItem[0].bslvalue;
				break;
			case 2:
				bsInfo.pv[0]=parItem[0].bshvalue;
				break;
			case 3:
				bsInfo.pv[1]=parItem[1].bslvalue;
				break;
			case 4:
				bsInfo.pv[1]=parItem[1].bshvalue;
				break;
		}
	}
	TMR1IE=0;
	for(i=0;i<CH_MAX;i++)
	{
		bsvalueH[i]=(unsigned char)(bsInfo.pv[i]>>3);
		bsvalueL[i]=(unsigned char)(bsInfo.pv[i]&0x07);
	}
	TMR1IE=1;
}

