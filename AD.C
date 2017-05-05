#include "define.h"
bank2 volatile _adInfo adInfo;
bank2 volatile _adInfo adInfoCopy;
bank2 unsigned char adCHS=0;
bank2 volatile unsigned int adint[]={0,0};
bank2 volatile unsigned char HotFlag=0;
bank2 volatile unsigned char HotInitFlag=3;
bank2 volatile float adfloat[3]={0,0,0};
bank2 volatile unsigned int adlostcount=0;
bank2 volatile unsigned int adhotcount=0;


bank2 volatile unsigned int adcoltime=1;
bank2 volatile unsigned int adcoltimecnt=0;


bank2 volatile unsigned int adfailcount=0;

//const double coeff[] =
//	{
//		 0.0000000000,	/* a0 */
//		 0.9999964239,	/* a1 */
//		-0.4998741238,	/* a2 */
//		 0.3317990258,	/* a3 */
//		-0.2407338084,	/* a4 */
//		 0.1676540711,	/* a5 */
//		-0.0953293897,	/* a6 */
//		 0.0360884937,	/* a7 */
//		-0.0064535442,	/* a8 */
//	};

void AD_Init(void)
{			
	unsigned char chartmp=0;

 adcoltime=1;
 adcoltimecnt=0;
	adCHS=0;
	SHT10_Init();	
	while(SHT10_ReadStatusReg((unsigned char *)&HotFlag))
	{
		SHT10_Init();
		chartmp++;
		if(chartmp>=INITLOSTMAX)
		{
			SHT10_ErrorFlag=1;
			break;
		}
	}
	SHT10_ReadCommand(adCHS);
}

void AD_Handle(void)
{	
//	int exp;
	float ftmp,ftmp1;
	unsigned char i;
	if(adlostcount<MS_1000)
	{	
		return;
	}	
	if(adcoltimecnt<adcoltime)
	{	
          adlostcount=MS_1000;
		return;
	}

	if(SHT10_SDA)
	{
		SHT10_SoftReset();		
		HotFlag=0;
        adfailcount++;
        adlostcount=0;
		goto loopreset;	//??????????????????????????????????????????
	}
	if(SHT10_ReadValue(adCHS,(unsigned int *)&adint[adCHS])) {adfailcount++;goto loopreset;}	//???????????????????????????????
	adCHS++;
	if(adCHS>=2)
	{
		adCHS=0;				
		//计算实际值
		ftmp=adint[0]*0.01;
		adfloat[0]=ftmp-40.0;
//		adfloat[0]=adint[0]*0.01-40.0;
	
		adfloat[1]=adint[1]*(-0.0000028);
		adfloat[1]+=0.0405;
		adfloat[1]*=adint[1];
		adfloat[1]+=(-4.0);
	//	adfloat[1]=-0.0000028*adint[1]*adint[1]+0.0405*adint[1]-4.0;

		ftmp=adfloat[0]-25;
		ftmp1=0.00008*adint[1];
		ftmp1+=0.01;
		ftmp*=ftmp1;
	//	ftmp*=(0.01+0.00008*adint[1]);
		adfloat[1]+=ftmp;
	//	adfloat[1]+=(adfloat[0]-25)*(0.01+0.00008*adint[1]);

		if(adfloat[1]>100)adfloat[1]=100;
		if(adfloat[1]<0.1)adfloat[1]=0.1;

/*露点计算  保留*/
		/********************下面几行用来求对数   考虑到堆栈不足 将系统函数库中的函数拆解出来*/
		/********************等同于下面屏蔽的第一句*/
		//		adfloat[2]=7.5*adfloat[0]/(237.3+adfloat[0])+(log10(adfloat[1])-2);
/*
		adfloat[2]=frexp(adfloat[1],&exp)*2.0-1.0;
		exp--;
		adfloat[2]= eval_poly(adfloat[2], coeff, sizeof coeff/sizeof coeff[0] - 1);
		adfloat[2]+=0.69314718055995 * exp;;
		adfloat[2]*=0.4342944819;
		adfloat[2]=7.5*adfloat[0]/(237.3+adfloat[0])+(adfloat[2]-2);
*/
		/***********************************************表达式结束*/
//		adfloat[2] = (adfloat[2] )*237.3/(7.5-adfloat[2]);	
		for(i=0;i<CH_MAX;i++)
		{
			adfloat[i]*=10;
			adfloat[i]+=parItem[i].sc;
			adInfoCopy.pv[i]=(int)(adfloat[i]);
		}	
		if(adInfoCopy.pv[1]>980)
		{
			if(!(HotFlag&0x04))
			{
				if(SHT10_WriteStatusReg(STATUSHOTEN))
				{		
					adfailcount++; 
                    goto loopreset; //?????????????????????????????????????????
				}				
				HotFlag|=0x04;			
				adInfo.pv[1]=adInfoCopy.pv[1];
				HotInitFlag=0;
				adhotcount=1;
			}
		}
		else
		{
			if(HotFlag&0x04)
			{	
				if(adInfoCopy.pv[0]>adInfo.pv[0]+20  || adhotcount>MS_2S)
				{		
					if(SHT10_WriteStatusReg(STATUSHOTDIS))
					{		
						adfailcount++;
                        goto loopreset; 	//???????????????????????????????????????????????????????
					}				
					HotFlag&=~0x04;					
					HotInitFlag|=0x04;	
					adhotcount=0;	
				}
			}
			else
			{
			//	if((adInfoCopy.pv[0]<adInfo.pv[0]+5)  && (HotInitFlag&0x04)) 
                if(HotInitFlag&0x04)
                	HotInitFlag|=0x02;
				if(HotInitFlag&0x03)
				{
					adInfo.pv[0]=adInfoCopy.pv[0]-8;
				    adInfo.pv[1]=adInfoCopy.pv[1]-20;//10代表1
					HotInitFlag&=0xfe;
				}
			}
		}		
	}
	//adlostcount=0;
    adcoltimecnt=0;
	SHT10_ErrorFlag = 0;
    adfailcount=0;
loopreset:	
    if(adfailcount>=3) SHT10_ErrorFlag = 1;
	if( SHT10_ErrorFlag ) { SHT10_SoftReset();	adInfo.pv[0]=-400; adInfo.pv[1]=0; adfailcount=3;}	
    adlostcount=0;
	SHT10_ReadCommand(adCHS);
 	SHT10_SDAEN_IN;  
}



