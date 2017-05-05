#include "define.h"

volatile _menuInfo menuInfo;
volatile unsigned char dippos;
volatile unsigned char starpos;
volatile int metervalue;
volatile unsigned int keycopy;
void Menu_Init(void);
void Menu_GetPar(void);
void Menu_KeyOnHandle(void);
int getLimit(void);
void keyContinueIf(void);
bit menuBefore(void);
bit menuOnIng(void);
void menuExit(void);
void menuInSet(void);
bit menuInShift(void);
void menuInAddDel(void);
void getstarDppos(void);
unsigned char getDippos(void);
void getNewdata(void);
void getLedshow(void);

/*************************************************************************************************************
函数名:Menu_Init
功能:菜单初始化
参数:无
返回:无
*************************************************************************************************************/
void Menu_Init(void)
{
	menuInfo.curState=1;
	menuInfo.parType=0;
	menuInfo.parChg=0;	
}
/*************************************************************************************************************
函数名:Menu_KeyOnHandle
功能:菜单操作
参数:无
返回:无
*************************************************************************************************************/
void Menu_KeyOnHandle(void)
{
	unsigned char newdata=0;
	int metervaluecopy=0;
	starpos=1;		
	if(menuInfo.curState==1)   //测量状态下  非SET键不进入参数
	{
		newdata=menuBefore();
		if(!newdata)return;				
		menuInfo.parTime=0;
	}
	else
	{			
		metervaluecopy=metervalue;
		newdata=menuOnIng();
		if(menuInfo.curState&0x80){Ht1621_WriteCommand(LCDTONEON);toneflag=2;return;}
	}		
	menuInfo.curState=2;
	keyContinueIf();
	if(newdata)//更新数标志
	{	
		getNewdata();
	}			
	getLedshow();	
	if(newdata || metervalue!=metervaluecopy ){Ht1621_WriteCommand(LCDTONEON);toneflag=2;}		
}
/*************************************************************************************************************
函数名:keyContinueIf
功能:按键连续按下判断
参数:无
返回:无
*************************************************************************************************************/
void keyContinueIf(void)
{	
	if(keyInfo.keyValue<0x100)
	{
		keyInfo.keyValue=0;	
	}
	else
	{
		if(keyInfo.keyValue!=keycopy )
		{ 
			flagtimes=0;//非上一次按键状态则清计数器
			keycopy=keyInfo.keyValue;
		}
		else 
		{
			if(flagtimes<200  && keyflag)flagtimes++;//当超过20后就再继续增加  防止数据溢出
		}
	}
	keyflag=0;	
}
/*************************************************************************************************************
函数名:menuBefore
功能:未进入菜单的按键处理
参数:无
返回:bit =0不进入菜单  =1进入菜单
*************************************************************************************************************/
bit menuBefore(void)
{
	if(!((keyInfo.keyValue&0x08)))return 0;
	menuInfo.parType=0;	
	menuInfo.parChg=0;
	return 1;
}
/*************************************************************************************************************
函数名:menuOnIng
功能:已进入菜单的按键处理
参数:无
返回:bit =0无新参数  =1新参数
*************************************************************************************************************/
bit menuOnIng(void)
{
	switch(keyInfo.keyValue)
	{
		case 0x100://连续按ADD  SUB按键事件 
		case 0x200:					
			if(!keyflag)return 0;			
			getstarDppos();							
		case 1:					   //加键		
		case 2:					  //减键
			menuInAddDel();					
			return 0;			
		case 0x800:		
			if(!keyflag)return 0;								
		case 8:	//SET					
			I2_Write(menu_par_ee_addr[menuInfo.parType]+menuInfo.parChg*2,(unsigned char *)(&metervalue),2);						  			
			if(menuInfo.parType==1)
			{
				*(&configItem.addr+menuInfo.parChg)=metervalue;			
			}
			else if(menuInfo.parType==2||menuInfo.parType==3)
			{
				*(&parItem[menuInfo.parType-2].sc+menuInfo.parChg)=metervalue;				
			}	
			else if(menuInfo.parType==parbiansongindex)
			{
				if(menuInfo.parChg<4)
				{
					*(&parItem[menuInfo.parChg/2].bslvalue+menuInfo.parChg%2)=metervalue;
					bsInfo.pv[menuInfo.parChg/2]=metervalue;
				}
				else if(menuInfo.parChg==4)
				{
					configItem.jzflag=metervalue;
				}
				else
				{
					configItem.outflag=metervalue;
				}
			}
	        else if(menuInfo.parType==collectcount  &&  menuInfo.parChg==3)	//修改采集时间
            {
                 adcoltime= metervalue;
                 I2_Write(menu_par_ee_addr[collectcount]+6,(unsigned char *)&metervalue,2);
            }			
			menuInSet();
			return 1;
		case 0x40:			   //移位500ms  强制退出参数
			menuExit();//
			return 0;					  
		case 0x04:	 			//移位键，参数返回上一个					
			return menuInShift();		
	}
	return 0;	
}	
/*************************************************************************************************************
函数名:menuExit
功能:退出菜单
参数:无
返回:无
*************************************************************************************************************/
void menuExit(void)
{
	menuInfo.curState=0x81;	
}
/*************************************************************************************************************
函数名:menuInShift
功能:菜单中SHIFT处理
参数:无
返回:bit=0 无处理  =1 新参数
*************************************************************************************************************/
bit menuInShift(void)
{
	if(menuInfo.parType==parhumiindex && menuInfo.parChg==3)
	{	
		menuInfo.parChg=0;	
	}
	else
	{
		if(menuInfo.parChg>0)
		{
			menuInfo.parChg--;	
		}
		else
			return 0;
	}
	return 1;
}
/*************************************************************************************************************
函数名:menuInSet
功能:菜单中SET处理
参数:无
返回:无
*************************************************************************************************************/
void menuInSet(void)
{
	unsigned char i=0;
	if((*(pmenu_par_dot[menuInfo.parType]+menuInfo.parChg)&0x80))
	{
		if(menuInfo.parType==0)
		{
			for(i=1;i<=menu_password[0];i++)//根据密码判断当前菜单
			{
				if(metervalue==menu_password[i])break;
			}
			if(i<=menu_password[0])
			{
				menuInfo.parType=i;			
				menuInfo.parChg=0;			
				return ;				
			}
			else
			{
				menuExit();
			}
		}
		else
		{						
			menuExit();
		}												
	}																						
	menuInfo.parChg++;	
	if(menuInfo.parType==parhumiindex && (menuInfo.parChg==1 || menuInfo.parChg==2))//如果是湿度变送上下限，跳过
	menuInfo.parChg=3;	
	i=menuInfo.parChg-1;
	if(*(pmenu_par_dot[menuInfo.parType]+i)&0x80)menuExit();			
}

/*************************************************************************************************************
函数名:menuInAddDel
功能:菜单中加减键处理
参数:无
返回:无
*************************************************************************************************************/
void menuInAddDel(void)
{
	int temp=1;
	unsigned char i;
	if(menuInfo.parType==collectcount && menuInfo.parChg!=3) return;//修改采集时间，出场日期不可以修改
	if(menuInfo.parType==parconfigindex && menuInfo.parChg==BAUD_ADDR)
	{/*波特率值特殊处理*/
		for(i=0;i<8;i++)
		{
			if(metervalue==baudcode[i]){temp=2;break;}
		}
		if(temp!=2)
		{
			i=7;
		}
		else
		{
			if((keyInfo.keyValue&0x02)||(keyInfo.keyValue&0x200))
			{
				if(i>0)i--;
			}
			else
			{
				if(i<7)i++;
			}
		}		
		metervalue=baudcode[i];
	}
	else
	{
		for(i=1;i<starpos;i++)				
			temp*=10;
		if((keyInfo.keyValue&0x02)||(keyInfo.keyValue&0x200))temp=-temp;	
		metervalue+=temp;  
	}
}
/*************************************************************************************************************
函数名:getstarDppos
功能:获取移位小数点位置
参数:无
返回:无
*************************************************************************************************************/
void getstarDppos(void)
{
	if(flagtimes>15){starpos=3;return;}
	if((menuInfo.parType==0)&&(menuInfo.parChg==0))
	{
		if((metervalue%10)==7)starpos=2;						
	}
	else
	{
		if((metervalue%10)==9 || (metervalue%10==-9))starpos=2;								
	}	
}	
/*************************************************************************************************************
函数名:getDippos
功能:获取当前参数小数点位置
参数:无
返回:无
*************************************************************************************************************/
unsigned char getDippos(void)
{
	unsigned char dipchar=0;
	dipchar=*(pmenu_par_dot[menuInfo.parType]+menuInfo.parChg)&0x0f;
	return dipchar;
}
/*************************************************************************************************************
函数名:getNewdata
功能:取新参数值
参数:无
返回:无
*************************************************************************************************************/
void getNewdata(void)
{
	//  菜单密码初始固化为300
	if((menuInfo.parType==0)&&(menuInfo.parChg==0))
	{	
		metervalue=1230;
		dippos=0;												
	}	
	else
	{						
		I2_Read(menu_par_ee_addr[menuInfo.parType]+menuInfo.parChg*2,(unsigned char *)(&metervalue),2); 	//读取新参数
		dippos=getDippos();			
	}
}
/*************************************************************************************************************
函数名:getLedshow
功能:获取窗口显示值
参数:无
返回:无
*************************************************************************************************************/
void getLedshow(void)
{
	unsigned char const *charshow;
	LED_DispChar(0,(unsigned char *)pmenu_par_item[menuInfo.parType]+menuInfo.parChg*4,0);//参数提示符
	getLimit();
	if(*(pmenu_par_dot[menuInfo.parType]+menuInfo.parChg)&0x20)
	{	
		// 以字符显示参数值
		if(menuInfo.parType==1)
			charshow=menu_item2[metervalue];
		else
			charshow=menu_item1[metervalue];	
		LED_DispChar(1,(unsigned char *)charshow,0);
	}
	else
	{
		if(menuInfo.parType==parbiansongindex)
		{
			bsInfo.pv[menuInfo.parChg/2]=metervalue;
		}
		LED_DispNumeric(1,metervalue,dippos);	
	}
}
/*************************************************************************************************************
函数名:getLimit
功能:判断参数值是否已达极限 
参数:无
返回:int 处理后的数值
*************************************************************************************************************/
int getLimit(void)
{
	if(metervalue>*(pmenu_par_max[menuInfo.parType]+menuInfo.parChg)) //限值判断  超过则按最大（小）处理
	{
		metervalue=*(pmenu_par_max[menuInfo.parType]+menuInfo.parChg);
	}
	if(metervalue<*(pmenu_par_min[menuInfo.parType]+menuInfo.parChg))
	{
		metervalue=*(pmenu_par_min[menuInfo.parType]+menuInfo.parChg);
	}	
}	