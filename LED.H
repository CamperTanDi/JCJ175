
#ifndef _LED_
#define _LED_

extern void LED_Init(void);
extern void LED_DispNumeric(char windows,int disp_val,unsigned char dotPos);
extern void LED_DispChar(char windows,char *pStrBuf,unsigned char dotPos);
extern void LED_DispUnit(unsigned char windows,unsigned char unit);
//extern void LED_DispLight1(unsigned char light,unsigned char lighten);
//extern void LED_DispLight2(unsigned char light,unsigned char lighten);
extern void LED_DispMode(unsigned char mode);
extern bank2 volatile unsigned char displaychar[];

#endif
