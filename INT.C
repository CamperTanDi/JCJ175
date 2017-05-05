#include "define.h"
void Timer0_Isr(void);
void Timer0_Init(void);
static void interrupt isr(void) 
{
	if((RCIE&&RCIF)||(TXIE&&TXIF))
	{
		if(RCIF)
		{
			if(getcount>=GETMAX)getcount=0;
			getdata[getcount]=RCREG;		
			getcount++;				
			rxtimer=0;
			
		}
		if(TXIF)
		{		
			if(sendcount>=sendmax)
			{
				sendcount=0;						
				TXIE=0;//禁止发送	
				TXEN=0;	
				SDI_RECEIVE;	
				CREN=1;	
				RCIE=1;		
				if(uartinitflag)uartinitflag=2;
			}
			else
			{			
				sendcount++;
				TXREG=senddata[sendcount];			
			}
		}	
	}	
	if(TMR1IF&&TMR1IE)
	{
        static unsigned int mstimecnt=0;
		TMR1IE=0;
		TMR1IF=0;
		//数据处理	

        mstimecnt++;
        if(mstimecnt>493)
        {
             adcoltimecnt++;
             mstimecnt=0;
         }

		bsindex++;
		if(bsindex>0x07)bsindex=0;
		if(bsindex<bsvalueL[0])CCPR1L=bsvalueH[0];
		else if(bsvalueH[0])CCPR1L=bsvalueH[0]-1;
		if(bsindex<bsvalueL[1])CCPR2L=bsvalueH[1];
		else if(bsvalueH[1])CCPR2L=bsvalueH[1]-1;
		//Uart0_Int()
		if(getcount>0)	
		{	
			rxtimer++;	
			if(rxtimer>waittime)
			{
				getover=1;			
			}	
		}
		else 
		{	
			if(rxtimer>0)rxtimer=0;	
		}		
		//复位脚
		if(KEY_RESET)
		{
			resetcount=0;
		}
		else
		{
			resetcount++;
		}
		//扫描按键
		if(menuInfo.curState&0x02)menuInfo.parTime++;
		keyflagtmer++;
		if(keyflagtmer>KEYCAP){keyflag=1;keyflagtmer=0; }	
		if(!KEY_SET)KeyCount[3]++;
		else KeyCount[3]=0;
		if(!KEY_ADD)KeyCount[0]++;
		else KeyCount[0]=0;
		if(!KEY_DEL)KeyCount[1]++;
		else KeyCount[1]=0;
		//初始TIMR1
		Inittimer++;
		adlostcount++;
		if( adhotcount>0)
        { 
            adhotcount++;
           if(adhotcount>=65530) 
               adhotcount=65530;
        }

  //报警闪烁和喇叭控制
        {
          static unsigned int t1=0;
          static unsigned int t2=0;
          static unsigned int t3=0;
          static unsigned int t4=0;

          if(alarmflag==1)
          {
             t1++;
             if(t1>=120)
             {
                 t1=0;
                 t2++;
                 if(keyon)
                   {
                     t3++;
                     if(t3>=45)
                        {
                           t3=0;
                           keyon=0;                      
                        }

                   }
             }

             if(t2%2 ==0 )
             {                                  
                  if(menuInfo.curState==1 &&  keyon==0 )
                     {
                         LIGHTOFF;
                     }
                  else
                      LIGHTON; 
              }
              else 
              { 
                   LIGHTON; 
              }
              
              if(t2==1) 
                 ALARMON;
              else if(t2==4) 
                 ALARMOFF;
              if(t2==5) 
                 t2=0;             
          }
          else if(alarmflag==3)
          {
             t1++;
             if(t1>=120)
             {
                 t1=0;
                 t2++;
                 if(keyon)
                   {
                     t3++;
                     if(t3>=45)
                        {
                           t3=0;
                           keyon=0;                      
                        }

                   }
             }

             if(t2%2 ==0 )
             {                                  
                  if(menuInfo.curState==1 &&  keyon==0 )
                     {
                         LIGHTOFF;
                     }
                  else
                      LIGHTON; 
              }
              else 
              { 
                   LIGHTON; 
              }
              
              if(t2==5) 
                 t2=0;   
             ALARMOFF;          
          }
          else if(alarmflag==2)
          {
               t4++;
               if(t4>1200) {ALARMON;LIGHTOFF;}
               if(t4>1600) {ALARMOFF;LIGHTON;t4=0;}
          }
          else if(toneflag)
          {
             t1++;
             ALARMON;
             if(t1>500)
             {
                  ALARMOFF;
                  t1=0;
                  toneflag=0;
             }
          }
          else 
          { 
             LIGHTON;ALARMOFF;
             t1=0;
           }

        }
		TMR1H=0xfd;
		TMR1L=0x80;
		TMR1IE=1;		
	}
}
void Timer0_Init(void)
{
	TMR1H=0xfd;
	TMR1L=0x80;
	TMR1IF=0;
	TMR1IE=1;	
	T1CON=0x21;
}
