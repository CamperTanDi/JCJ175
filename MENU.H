
#ifndef _MENU_
#define _MENU_

typedef  struct {
	unsigned char curState;						//当前运行的状态,bit8=1从参数菜单退回到测量菜单,bit0-bit7:=1测量状态,=2参数状态
	unsigned char parType;						//第x组参数
	unsigned char parChg;							//参数项
	unsigned int  parTime;						//
}_menuInfo;

extern volatile _menuInfo menuInfo;

/**************************************************************
	菜单初时化

**************************************************************/


/**************************************************************

**************************************************************/
extern void Menu_KeyOnHandle(void);
extern void Menu_ExitPar(void);
extern void Menu_Init(void);

#endif
