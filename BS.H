#ifndef _BS_
#define _BS_

extern void BS_Init(void);
extern void BS_Handle(void);
typedef struct{
	int pv[CH_MAX];	
}_bsInfo;

extern bank2 volatile _bsInfo bsInfo;
extern bank2 volatile unsigned char bsvalueH[2];
extern bank2 volatile unsigned char bsvalueL[2];
extern bank2 volatile unsigned char bsindex;

#endif 