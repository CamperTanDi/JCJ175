#include "define.h"
#define LEDMAX 11
#define LED_DP 0x80
#define LED_G  0x20
bank2 volatile unsigned char displaychar[]={0,0,0,0,0,0,0,0,0,0,0};	
const unsigned char shuzi[]={0x5F,0x50,0x3D,0x79,0x72,0x6B,0x6F,0x51,0x7F,0x7B};
const unsigned char zimu[]={0x77,0x6E,0x2C,0x7C,0x2F,0x27,0x7B,0x76,0x50,0x58,0x66,0xE,0x65,0x64,0x6C,0x37,0x73,0x24,0x6B,0x2E,0x5E,0x4C,0x1A,0x76,0x7A,0x3d};
const unsigned char  LedIndex[]={10,9,8,7,6,5,4,3};
const unsigned char PosUnit[][2]={{2,2},{1,6}};
const unsigned char ValueUnit[][2]={{0x80,0x08},{0x08,0x08}};
//const unsigned char dispUnit[]={2,1};
enum{NONE,HUMIUNIT,TEMPUNIT};

/*************************************************************************************************************
函数名:LED_Init
功能:显示及对应变量初始化
参数:无
返回:无
*************************************************************************************************************/
void LED_Init(void)
{	
	Ht1621_Init();
}
/*************************************************************************************************************
函数名:LED_DispUnit
功能:显示单位
参数:windows:			显示窗口,范围0-1
	 unit   :           单位符号  0:无  1:湿度 2:温度
返回:无
*************************************************************************************************************/
void LED_DispUnit(unsigned char windows,unsigned char unit)
{	
	switch(unit)
	{
		case NONE:
			displaychar[PosUnit[windows][0]]&=~ValueUnit[windows][0];
			displaychar[PosUnit[windows][1]]&=~ValueUnit[windows][1];	
			break;
		case HUMIUNIT:
			displaychar[PosUnit[windows][0]]|=ValueUnit[windows][0];
			displaychar[PosUnit[windows][1]]&=~ValueUnit[windows][1];	
			break;
		case TEMPUNIT:
			displaychar[PosUnit[windows][0]]&=~ValueUnit[windows][0];
			displaychar[PosUnit[windows][1]]|=ValueUnit[windows][1];	
			break;
	}
}
/*************************************************************************************************************
函数名:LED_DispLight1
功能:显示光柱1
参数:light  :			光柱值
	 lighten:           使能位 =1有效
返回:无
*************************************************************************************************************/
/*void LED_DispLight1(unsigned char light,unsigned char lighten)
{
	const unsigned char LightCode[]={0x80,0xc0,0xe0,0xf0,0xf8,0xf9};
	displaychar[0]&=0x06;
	if(lighten)
	{		
		displaychar[0]|=LightCode[light>5?5:light];
	}		
}
*/
/*************************************************************************************************************
函数名:LED_DispLight2
功能:显示光柱2
参数:light  :			光柱值
	 lighten:           使能位 =1有效
返回:无
*************************************************************************************************************/
/*void LED_DispLight2(unsigned char light,unsigned char lighten)
{
	const unsigned char LightCode[]={0x80,0x90,0xb0,0xf0};
	displaychar[1]&=0x0f;
	if(lighten)
	{		
		displaychar[1]|=LightCode[light>3?3:light];
	}		
}*/
/*************************************************************************************************************
函数名:LED_DispMode
功能:显示控制模式
参数:mode   :			模式值  
	 bit7 手动
	 bit6 自动
	 bit5 加热
	 bit4 排水
	 bit3 加水
	 bit2 制冷
	 bit1 加湿
	 bit0 除湿
	 =1有效
返回:无
*************************************************************************************************************/
void LED_DispMode(unsigned char mode)
{
	const unsigned char AutoManCode[]={0x04,0x02};
	const unsigned char ControlCode[]={0x04,0x02,0x01,0x40,0x20,0x10};
	displaychar[0]&=0xf9;
	displaychar[1]&=0xf8;
	displaychar[2]&=0x8f;	
	if(mode&0x80)displaychar[0]|=AutoManCode[0];
	if(mode&0x40)displaychar[0]|=AutoManCode[1];
	if(mode&0x20)displaychar[1]|=ControlCode[0];
	if(mode&0x10)displaychar[1]|=ControlCode[1];
	if(mode&0x08)displaychar[1]|=ControlCode[2];
	if(mode&0x04)displaychar[2]|=ControlCode[3];
	if(mode&0x02)displaychar[2]|=ControlCode[4];
	if(mode&0x01)displaychar[2]|=ControlCode[5];		
}
/*************************************************************************************************************
函数名:LED_DispNumeric
功能:窗口数字显示
参数:windows:			显示窗口,范围0-1
	 dispVal:			显示数值
	 dotGlintPos:		小数点动态显示位置
	 dotPos:			小数点显示位置
返回:无
*************************************************************************************************************/
void LED_DispNumeric(char windows,int disp_val,unsigned char dotPos)
{
   	unsigned char i=0,j=0;
	unsigned char disptmp=0;
	unsigned int temp=0;
	unsigned char flag=0;
	unsigned char c=0;	
	if(disp_val<0)
	{
		disp_val=-disp_val;	
		if(windows==0)
		{
			disp_val=0;
		}
		else
		{
			if(disp_val>1999)
			{
				disp_val=200;
				dotPos=0;
			}	
			flag=1;//负号标志
		}
	}
	if(disp_val>999)
	{
		if(windows==0)
		{
			if(disp_val>9999)
			{
				disp_val+=50;
				disp_val/=100;
			}
			else
			{
				disp_val+=5;
				disp_val/=10;
			}
			dotPos=0;
		}
		else
		{			
			if(disp_val>9999)
			{
				disp_val+=5;
				disp_val/=10;
				dotPos=0;
			}					
		}		
		flag|=2;		
	}
	temp=1000;
	for(i=0;i<4;i++)
	{ 	
		j=disp_val/temp;
		disp_val%=temp;
		temp/=10;
		disptmp=shuzi[j];
		if(flag&0x01 && disptmp==shuzi[0] && dotPos==3 && i==0 ){disptmp=0;c=1;}
		if(j)c=1;
		if((c==0)&&(i<3-dotPos)&&(!(flag&0x02)))disptmp=0;
		if((i==3-dotPos)&&(dotPos))disptmp|=LED_DP;
		if((i==0)&&(flag&0x01))disptmp|=LED_G;			
		displaychar[LedIndex[i+windows*4]]&=0x0f;
		displaychar[LedIndex[i+windows*4]]|=disptmp<<4;	
		displaychar[LedIndex[i+windows*4]-1]&=0xf0;
		displaychar[LedIndex[i+windows*4]-1]|=disptmp>>4;	
	}
}

/*************************************************************************************************************
函数名:LED_DispChar
功能:窗口字符显示
参数:windows:			显示窗口,范围0-1
	 pStrBuf:			显示字符串指针
	 dotGlintPos:		小数点动态显示位置
	 dotPos:			小数点显示位置	 
返回:无
*************************************************************************************************************/
void LED_DispChar(char windows,char *pStrBuf,unsigned char dotPos)
{
	unsigned char i=0;
	unsigned char charled=0;
	unsigned char disptmp=0;		
	for(i=0;i<4;i++)
	{			
		charled=*(pStrBuf+i);
		if((charled>=0x30)&&(charled<=0x39))			 //数字
		{
			disptmp=shuzi[charled-0x30];
		}
	//	else if((charled>=0x61)&&(charled<=0x7a))		 //小写字母
	//	{
	//		disptmp=zimu[charled-0x61];
	//	}
		else if((charled>=0x41)&&(charled<=0x5a))		 //大写字母
		{
			disptmp=zimu[charled-0x41]; 
		}	
		else if(charled==0x2d)//-号
		{
			disptmp=LED_G;	
		}
		else
		{
			disptmp=0x0;
		}
		if((dotPos)&&(3-dotPos==i))disptmp|=LED_DP;//小数点位置	
		displaychar[LedIndex[i+windows*4]]&=0x0f;
		displaychar[LedIndex[i+windows*4]]|=disptmp<<4;
		displaychar[LedIndex[i+windows*4]-1]&=0xf0;
		displaychar[LedIndex[i+windows*4]-1]|=disptmp>>4;
	}
}





