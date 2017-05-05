#include "define.h"

//volatile unsigned char ahflag,alflag;
void Alarm_Init(void);
void Alarm_Handle(void);

volatile unsigned char alarmflag=0;  //=0 bu bao jing   =1baojing shan =2zhi shan
volatile unsigned char keyalarm[4]={0,0,0,0};
volatile unsigned char keyflagalalrm=0;

void Alarm_Init(void)
{
//	ahflag=0;
//	alflag=0;
	THAEN_OUT;
	TLAEN_OUT;
	HHAEN_OUT;
	HLAEN_OUT;
	THA=0;
	TLA=0;
	HHA=0;
	HLA=0;

	ALARM_OUT;
    LIGHT_OUT;
    ALARMOFF;
    LIGHTON;
    keyalarm[0]=0;
    keyalarm[1]=0;
    keyalarm[2]=0;
    keyalarm[3]=0;
    alarmflag=0;
    keyflagalalrm=0;
}

void Alarm_Handle(void)
{	
    static unsigned char altmp[4]={0,0,0,0};
	int pv=0;
	unsigned char index=0;


if(SHT10_ErrorFlag) //如果传感器探头坏掉，关闭所有的继电器，开启声音和闪屏报警
{
	THA=0;
	TLA=0;
	HHA=0;
	HLA=0;
    alarmflag=2;
    return;
}


	for(index=0;index<CH_MAX;index++)
	{	
		pv=adInfo.pv[index];//判断报警状态  并设置报警标志位
		if(pv>(parItem[index].ha+parItem[index].yh))//上限报警置位
		{
		    altmp[index*2]=1;   if( !keyalarm[index*2]) {keyalarm[0]=0;  keyalarm[1]=0;  keyalarm[2]=0;  keyalarm[3]=0; }
		}

		if(pv<(parItem[index].ha-parItem[index].yh))//上限报警解除
		{
			altmp[index*2]=0;  keyalarm[index*2]=0;
		}

		if(pv>(parItem[index].la+parItem[index].yl))//下限报警解除
		{
			altmp[index*2+1]=0;  keyalarm[index*2+1]=0;
		}

		if(pv<(parItem[index].la-parItem[index].yl))//下限报警置位
		{
			altmp[index*2+1]=1;   if( !keyalarm[index*2+1])  {keyalarm[0]=0;  keyalarm[1]=0;  keyalarm[2]=0;  keyalarm[3]=0; }
		}	
	}	
	THA=altmp[0];
	TLA=altmp[1];
	HHA=altmp[2];
	HLA=altmp[3];  



   // if((ahflag || alflag)&&configItem.spe==0)    
    if( (altmp[0] || altmp[1] || altmp[2] || altmp[3]) &&configItem.spe==0)  
    {  
         if(keyflagalalrm)   
         {
              if(keyalarm[0]!=1)  keyalarm[0]=1;
              if(keyalarm[1]!=1)  keyalarm[1]=1;
              if(keyalarm[2]!=1)  keyalarm[2]=1;
              if(keyalarm[3]!=1)  keyalarm[3]=1;
         }
          if ( keyalarm[0] || keyalarm[1] || keyalarm[2] || keyalarm[3])   
                alarmflag=3;    
          else  alarmflag=1;          
    }
    else  
    {
         alarmflag=0;  keyalarm[0]=0;  keyalarm[1]=0;  keyalarm[2]=0;  keyalarm[3]=0;
    }
    keyflagalalrm=0;


  /*
	if((ahflag || alflag)&&configItem.spe==0)    
         alarmflag=1;    
     else  
         alarmflag=0;
*/
}


