// 2014-10-22 

#include "define.h"
 __CONFIG(0xe002);
 __CONFIG(0x3fff);
volatile _parItem parItem[CH_MAX];
volatile _configItem configItem;
void Disp_Handle(void);
void GetAllPar(void);
void Meter_Init(void);
void Reset_Init(void);
void Reset_Handle(void);
void Menu_ExitPar(void);
volatile unsigned int resetcount=0;
volatile unsigned int Inittimer=0;
volatile unsigned char exitflag=1;
volatile unsigned char toneflag=0;
volatile unsigned char  getkeyvalue=0;

//28个参数，56个字节
const int par1reset[]=
{
	1,9600,0,//地址，波特率，报警开关
	0,-400,800,800,0,-400,0,//温度修正，变送下限，变送上限，报警上限，回差，报警下限，回差
	0,0,1000,1000,0,0,0,	//湿度修正，变送下限，变送上限，报警上限，回差，报警下限，回差
	350,1650,350,1650,0,1,  //温度变送低端，温度变送高端，湿度变送低端，湿度变送高端,jzflag,outflag
	2014,9,10,1,14  		//生产日期年月日，采集时间，14未知
};   
const unsigned char _PV[]={' ',' ',' ',' '};
const unsigned char _SV[]={'P','A','S','S'};
const unsigned char _END[]={' ','E','N','D'};
const unsigned char _NAME[]={' ','J','C','J'};
const unsigned char _VERSION[]={' ','1','7','5'};
const unsigned char _ERROR[]={' ','E','R','R'};
void main(void)
{	
	ANSEL=0;ANSELH=0;//关闭所有AD输入	
	Reset_Init();
	Timer0_Init();
	I2_Init();	
	AD_Init();	
	LED_Init();
	BS_Init();	
	Key_Init();
	Alarm_Init();
	Menu_Init();
	Meter_Init();
	GetAllPar();	
	PEIE=1;//外围接口中断使能位置位
	GIE=1;//总中断使能位置位
	LED_DispChar(0,(unsigned char *)_NAME,0);
	LED_DispChar(1,(unsigned char *)_VERSION,0); 	
	Ht1621_Disp((unsigned char *)displaychar);
	Inittimer=0;
	while(Inittimer<MS_INIT+MS_2S){ adcoltimecnt=adcoltime ;AD_Handle(); }
	LED_DispChar(0,(unsigned char *)_PV,0);
	LED_DispChar(1,(unsigned char *)_SV,0); 	
	Ht1621_Disp((unsigned char *)displaychar);
	Inittimer=0;
	while(Inittimer<MS_2S){ adcoltimecnt=adcoltime ;AD_Handle(); }
	WDTCON=0x0b;
	//////////////////////调试//////////////////
/*	GIE=0;
	while(1)
	{
		//asm("clrwdt");
        ALARMON;
		DelayMS(1);
		ALARMOFF;
		DelayMS(1);
		
	}*/
	////////////////////////////////////////////	
	while(1)
	{
		asm("clrwdt");	
		if(exitflag)
		{
			if(Inittimer>=MS_INIT)
			{
				exitflag=0;				
				Uart0_Init();
			}		
		}
		else	
		{	
			if(resetcount>=2000){Reset_Handle();resetcount=0;uartinitflag=2;}
			if(uartinitflag==2){Uart0_Init();uartinitflag=0;}	
			if(getover==1){Uart0_Handle();getover=0;}						
			Disp_Handle();
			{				
				if((menuInfo.parTime>MS_EXITPAR)&&((menuInfo.curState&0x02)&&(menuInfo.parType!=parbiansongindex)))Menu_ExitPar();		
                getkeyvalue=Key_GetKey();
                if(menuInfo.parType==0) keyflagalalrm=getkeyvalue;
    
             	if(getkeyvalue)
                //if(keyflagalalrm)
				{									
				//	T0IE=0;
					menuInfo.parTime=0;
				//	T0IE=1;					
					Menu_KeyOnHandle();						
					if(menuInfo.curState & 0x80)				//退出参数
					{	
						Menu_ExitPar();	 				
					}							
				}
			}
		}			
		AD_Handle();	
		BS_Handle();
		Alarm_Handle();	
		if(toneflag){toneflag--;if(!toneflag)Ht1621_WriteCommand(LCDTONEOFF);} 	
	} 	
}

void Disp_Handle(void)
{	
	unsigned char mode=0;
	if(SHT10_ErrorFlag && (menuInfo.curState&0x01))
	{
		LED_DispChar(0,(unsigned char *)_ERROR,0);
		LED_DispChar(1,(unsigned char *)_ERROR,0);
	}
	else
	{		
		if(THA)mode|=0x04;
		if(TLA)mode|=0x20;
		if(HHA)mode|=0x01;
		if(HLA)mode|=0x02;
		if(menuInfo.curState&0x01)
		{		
			LED_DispNumeric(1,adInfo.pv[0],1);
			LED_DispNumeric(0,adInfo.pv[1],1);		
			LED_DispUnit(1,1);
			LED_DispUnit(0,2);
		//	LED_DispLight1(2,0);
		//	LED_DispLight2(2,0);	
			mode|=0x40;				
		}
		else
		{
			if(menuInfo.parType==2)
			{
				if(menuInfo.parChg==3 || menuInfo.parChg==4)
					mode|=0x04;
				if(menuInfo.parChg==5 || menuInfo.parChg==6)
					mode|=0x20;
			}
			if(menuInfo.parType==3)
			{
				if(menuInfo.parChg==3 || menuInfo.parChg==4)
					mode|=0x01;
				if(menuInfo.parChg==5 || menuInfo.parChg==6)
					mode|=0x02;
			}
			LED_DispUnit(1,0);
			LED_DispUnit(0,0);	
		}
		LED_DispMode(mode);
	}	
	Ht1621_Disp((unsigned char *)displaychar);
}

void GetAllPar(void)
{	  	
	unsigned char i=0;
	for(i=0;i<CH_MAX;i++)
	{		
		I2_Read(menu_par_ee_addr[2+i],(unsigned char *)&parItem[i].sc,14);//从地址8开始读，7个参数   温度修正，变送下限，变送上限，上限报警值，上限回差，下限报警值，下限回差
																		  //从地址22开始读7个参数，	 湿度修正，变送下限，变送上限，上限报警值，上限回差，下限报警值，下限回差
		I2_Read(menu_par_ee_addr[parbiansongindex]+i*4,(unsigned char *)&parItem[i].bslvalue,4);//parbiansongindex=4,从36开始读2个参数，从40开始读2个参数
																								//温度变送低端，温度变送高端，湿度变送低端，湿度变送高端				
	}
	I2_Read(menu_par_ee_addr[parconfigindex],(unsigned char *)&configItem.addr,6);//parconfigindex=1,从2开始读3个参数，地址，波特率，报警开关
	I2_Read(menu_par_ee_addr[parbiansongindex]+8,(unsigned char *)&configItem.jzflag,4);//从44开始读两个参数jzflag outflag
    I2_Read(menu_par_ee_addr[collectcount]+6,(unsigned char *)&adcoltime,2);//collectcount=5,从54开始读一个参数，采集时间间隔
	parItem[1].bsl=0;parItem[1].bsh=1000;//湿度保持不变
}	
/***********************************************************************************
***********************************************************************************/
void Meter_Init(void)
{
	int meterinit=0;	
	I2_Read(0xfe,(unsigned char *)&meterinit,2);//读取标志，在倒数第二个字节
	if(meterinit!=3007)
	{	
		I2_Write(menu_par_ee_addr[1],(unsigned char *)par1reset,sizeof(par1reset));	//从地址2开始写入28个参数56个字节	
		meterinit=3007;		
		I2_Write(0xfe,(unsigned char *)&meterinit,2);//写入标志			
	}			
}
void Menu_ExitPar(void)
{
	menuInfo.curState=1;
	menuInfo.parType=0;
	LED_DispChar(0,(unsigned char *)_END,0);	
	Ht1621_Disp((unsigned char *)displaychar);
	Inittimer=0;	
	exitflag=1;
}	

void Reset_Init(void)
{
	KEY_RESETEN_IN;
}
void Reset_Handle(void)
{	
	configItem.addr=1;
	configItem.baud=9600;
	I2_Write(menu_par_ee_addr[parconfigindex],(unsigned char *)&configItem.addr,4);	//parconfigindex=1
} 