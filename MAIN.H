#ifndef _MAIN_
#define _MAIN_
#define KEY_RESET 		RB0
#define KEY_RESETEN 	TRISB0
#define KEY_RESETEN_OUT KEY_RESETEN=0
#define KEY_RESETEN_IN  KEY_RESETEN=1
extern volatile unsigned int resetcount;
extern volatile unsigned int Inittimer;
extern volatile unsigned char toneflag;
#endif