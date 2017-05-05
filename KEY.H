
#ifndef _KEY_
#define _KEY_
/*******************************************************************
keyValue:
	=1 add on =2 sub_on,=4 shift_on,=8 set_on,
	=0x10 add_500ms,=0x20 sub_500ms,=0x40 shift_500ms，=0x80 set_500ms,
	=0x100,add>500ms  =0x200,sub>500ms,=0x400,shift>500ms,=0x800,set>500ms
keyStatus:已置位标志  依次对应keyValue的前10项
******************************************************************/
typedef struct {
	unsigned int 	keyValue;			
	unsigned int    keyStatus;													
}_keyInfo;		

extern bank3 volatile _keyInfo keyInfo;
extern  bit  Key_GetKey(void); 
extern void AD_Out(char index);
extern void Key_Init(void);
extern bank1 volatile unsigned int  KeyCount[4];
extern bank3 volatile unsigned char keyflag;
extern bank3 volatile unsigned char flagtimes;
extern bank3 volatile unsigned char keyflagtmer;
extern bank3 volatile unsigned char keyon; //判断是否有按键按下
//const unsigned char keyIndex[];
#define KEY_SET RB3
#define KEY_ADD RB2
#define KEY_DEL RB1
#define KEY_SETEN TRISB3
#define KEY_ADDEN TRISB2
#define KEY_DELEN TRISB1
#define KEY_SETEN_IN KEY_SETEN=1
#define KEY_ADDEN_IN KEY_ADDEN=1
#define KEY_DELEN_IN KEY_DELEN=1
#endif
