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
������:Menu_Init
����:�˵���ʼ��
����:��
����:��
*************************************************************************************************************/
void Menu_Init(void)
{
	menuInfo.curState=1;
	menuInfo.parType=0;
	menuInfo.parChg=0;	
}
/*************************************************************************************************************
������:Menu_KeyOnHandle
����:�˵�����
����:��
����:��
*************************************************************************************************************/
void Menu_KeyOnHandle(void)
{
	unsigned char newdata=0;
	int metervaluecopy=0;
	starpos=1;		
	if(menuInfo.curState==1)   //����״̬��  ��SET�����������
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
	if(newdata)//��������־
	{	
		getNewdata();
	}			
	getLedshow();	
	if(newdata || metervalue!=metervaluecopy ){Ht1621_WriteCommand(LCDTONEON);toneflag=2;}		
}
/*************************************************************************************************************
������:keyContinueIf
����:�������������ж�
����:��
����:��
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
			flagtimes=0;//����һ�ΰ���״̬���������
			keycopy=keyInfo.keyValue;
		}
		else 
		{
			if(flagtimes<200  && keyflag)flagtimes++;//������20����ټ�������  ��ֹ�������
		}
	}
	keyflag=0;	
}
/*************************************************************************************************************
������:menuBefore
����:δ����˵��İ�������
����:��
����:bit =0������˵�  =1����˵�
*************************************************************************************************************/
bit menuBefore(void)
{
	if(!((keyInfo.keyValue&0x08)))return 0;
	menuInfo.parType=0;	
	menuInfo.parChg=0;
	return 1;
}
/*************************************************************************************************************
������:menuOnIng
����:�ѽ���˵��İ�������
����:��
����:bit =0���²���  =1�²���
*************************************************************************************************************/
bit menuOnIng(void)
{
	switch(keyInfo.keyValue)
	{
		case 0x100://������ADD  SUB�����¼� 
		case 0x200:					
			if(!keyflag)return 0;			
			getstarDppos();							
		case 1:					   //�Ӽ�		
		case 2:					  //����
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
	        else if(menuInfo.parType==collectcount  &&  menuInfo.parChg==3)	//�޸Ĳɼ�ʱ��
            {
                 adcoltime= metervalue;
                 I2_Write(menu_par_ee_addr[collectcount]+6,(unsigned char *)&metervalue,2);
            }			
			menuInSet();
			return 1;
		case 0x40:			   //��λ500ms  ǿ���˳�����
			menuExit();//
			return 0;					  
		case 0x04:	 			//��λ��������������һ��					
			return menuInShift();		
	}
	return 0;	
}	
/*************************************************************************************************************
������:menuExit
����:�˳��˵�
����:��
����:��
*************************************************************************************************************/
void menuExit(void)
{
	menuInfo.curState=0x81;	
}
/*************************************************************************************************************
������:menuInShift
����:�˵���SHIFT����
����:��
����:bit=0 �޴���  =1 �²���
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
������:menuInSet
����:�˵���SET����
����:��
����:��
*************************************************************************************************************/
void menuInSet(void)
{
	unsigned char i=0;
	if((*(pmenu_par_dot[menuInfo.parType]+menuInfo.parChg)&0x80))
	{
		if(menuInfo.parType==0)
		{
			for(i=1;i<=menu_password[0];i++)//���������жϵ�ǰ�˵�
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
	if(menuInfo.parType==parhumiindex && (menuInfo.parChg==1 || menuInfo.parChg==2))//�����ʪ�ȱ��������ޣ�����
	menuInfo.parChg=3;	
	i=menuInfo.parChg-1;
	if(*(pmenu_par_dot[menuInfo.parType]+i)&0x80)menuExit();			
}

/*************************************************************************************************************
������:menuInAddDel
����:�˵��мӼ�������
����:��
����:��
*************************************************************************************************************/
void menuInAddDel(void)
{
	int temp=1;
	unsigned char i;
	if(menuInfo.parType==collectcount && menuInfo.parChg!=3) return;//�޸Ĳɼ�ʱ�䣬�������ڲ������޸�
	if(menuInfo.parType==parconfigindex && menuInfo.parChg==BAUD_ADDR)
	{/*������ֵ���⴦��*/
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
������:getstarDppos
����:��ȡ��λС����λ��
����:��
����:��
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
������:getDippos
����:��ȡ��ǰ����С����λ��
����:��
����:��
*************************************************************************************************************/
unsigned char getDippos(void)
{
	unsigned char dipchar=0;
	dipchar=*(pmenu_par_dot[menuInfo.parType]+menuInfo.parChg)&0x0f;
	return dipchar;
}
/*************************************************************************************************************
������:getNewdata
����:ȡ�²���ֵ
����:��
����:��
*************************************************************************************************************/
void getNewdata(void)
{
	//  �˵������ʼ�̻�Ϊ300
	if((menuInfo.parType==0)&&(menuInfo.parChg==0))
	{	
		metervalue=1230;
		dippos=0;												
	}	
	else
	{						
		I2_Read(menu_par_ee_addr[menuInfo.parType]+menuInfo.parChg*2,(unsigned char *)(&metervalue),2); 	//��ȡ�²���
		dippos=getDippos();			
	}
}
/*************************************************************************************************************
������:getLedshow
����:��ȡ������ʾֵ
����:��
����:��
*************************************************************************************************************/
void getLedshow(void)
{
	unsigned char const *charshow;
	LED_DispChar(0,(unsigned char *)pmenu_par_item[menuInfo.parType]+menuInfo.parChg*4,0);//������ʾ��
	getLimit();
	if(*(pmenu_par_dot[menuInfo.parType]+menuInfo.parChg)&0x20)
	{	
		// ���ַ���ʾ����ֵ
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
������:getLimit
����:�жϲ���ֵ�Ƿ��ѴＫ�� 
����:��
����:int ���������ֵ
*************************************************************************************************************/
int getLimit(void)
{
	if(metervalue>*(pmenu_par_max[menuInfo.parType]+menuInfo.parChg)) //��ֵ�ж�  ���������С������
	{
		metervalue=*(pmenu_par_max[menuInfo.parType]+menuInfo.parChg);
	}
	if(metervalue<*(pmenu_par_min[menuInfo.parType]+menuInfo.parChg))
	{
		metervalue=*(pmenu_par_min[menuInfo.parType]+menuInfo.parChg);
	}	
}	