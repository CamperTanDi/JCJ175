#ifndef _AD_
#define _AD_

extern void AD_Init(void);
extern void AD_Handle(void);
typedef struct{
	int pv[CH_MAX+1];										//pv信号值	 
	unsigned char status[CH_MAX];			//bit0=1 high_over bit1=1 low_over bit2=1 开路
	unsigned char newch;							//bit7=1:new data
}_adInfo;
extern bank2 volatile _adInfo adInfo;
extern bank2 volatile _adInfo adInfoCopy;
extern bank2 unsigned char adCHS;
extern bank2 volatile unsigned int adint[];
extern bank2 volatile unsigned char HotInitFlag;
extern bank2 volatile float adfloat[3];
extern bank2 volatile unsigned int adlostcount;
extern bank2 volatile unsigned int adhotcount;
extern bank2 volatile unsigned int adcoltime;
extern bank2 volatile unsigned int adcoltimecnt;
#endif
