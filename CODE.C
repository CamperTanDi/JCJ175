//#include "define.h"

#include "code.h"

/*起始地址数据为密码个数N，后面连续N个数据为对应密码值*/
const unsigned int menu_password[]={5,1234,1236,1238,1277,1287};
const unsigned char menu_par0_item[][4]= 
{
	' ',' ','C','D'
};
const unsigned char menu_par1_item[][4]=
{
	{' ','A','D','R'},
	{' ','B','U','D'},	
	{' ','S','P','E'}		
};
const unsigned char menu_par2_item[][4]= 
{		
	{' ','T','S','C'},
	{' ','T','B','L'},
	{' ','T','B','H'},
	{' ','T','H','A'},
	{' ','T','Y','H'},
	{' ','T','L','A'},
	{' ','T','Y','L'}
};
const unsigned char menu_par3_item[][4]=
{
	{' ','H','S','C'},
	{' ','H','B','L'},
	{' ','H','B','H'},
	{' ','H','H','A'},
	{' ','H','Y','H'},
	{' ','H','L','A'},
	{' ','H','Y','L'}	
};
const unsigned char menu_par4_item[][4]=
{
	{' ','T','0','4'},
	{' ','T','2','0'},
	{' ','H','0','4'},
	{' ','H','2','0'},
	{' ','J','Z',' '},
	{' ','O','U','T'}
};
const unsigned char menu_par5_item[][4]=
{
	{' ','Y','A','R'},
	{' ','M','O','N'},
	{' ','D','A','Y'},
	{' ','C','O','L'}
};



const int menu_par0_max[]=
{
	9999/*CD*/
};
const int menu_par1_max[]=
{
	255,19200,1
};
const int menu_par2_max[]=
{
	1000,1500,1500,1000,1000,1000,1000
};
const int menu_par3_max[]=
{
	1000,1000,1000,1000,1000,1000,1000
};
const int menu_par4_max[]=
{                                             
	2000,2000,2000,2000,0,1
};
const int menu_par5_max[]=
{
	9999/*year*/,12/*mon*/,31/*day*/,9999
};


const int menu_par0_min[]=
{
	0/*CD*/
};
const int menu_par1_min[]=
{
	1,300,0
};
const int menu_par2_min[]=
{
	-999,-999,-999,-999,0,-999,0
};
const int menu_par3_min[]=
{	
	-999,0,0,0,0,0,0
};
const int menu_par4_min[]=
{	
	1,1,1,1,0,0
};
const int menu_par5_min[]=
{
	2010/*year*/,1/*mon*/,1/*day*/,1
};

//bit6=1 开始   bit7=1 结束  bit5=1下窗口显示字符 
// bit[0:2]=0-无小数点 4-跟随DIP变化 
//=5:与sn有关,热电偶/热电阻(dip==0)?0:1，线性电压/线性电流随dip
const unsigned char  menu_par0_dot[]={0xc0/*CA*/};
const unsigned char  menu_par1_dot[]=    
{
	0x40,0,0xa0
};
const unsigned char  menu_par2_dot[]=
{
	0x41,0x01,0x01,0x01,0x01,0x01,0x81
};
const unsigned char  menu_par3_dot[]=
{
	0x41,0x01,0x01,0x01,0x01,0x01,0x81
};
const unsigned char  menu_par4_dot[]=
{
	0x40,0,0,0,0,0xa0
};
const unsigned char  menu_par5_dot[]=
{
	0x40/*year*/,0/*mon*/,0/*day*/,0x80
};
	
							
const unsigned char *const pmenu_par_item[]=
{
	menu_par0_item[0],menu_par1_item[0],menu_par2_item[0],menu_par3_item[0],
	menu_par4_item[0],menu_par5_item[0]
};

const int *const pmenu_par_max[]=
{
	menu_par0_max,menu_par1_max,menu_par2_max,menu_par3_max,
	menu_par4_max,menu_par5_max
};
	
const int *const pmenu_par_min[]=
{
	menu_par0_min,menu_par1_min,menu_par2_min,menu_par3_min,
	menu_par4_min,menu_par5_min
};

const unsigned char *const pmenu_par_dot[]={
	menu_par0_dot,menu_par1_dot,menu_par2_dot,menu_par3_dot,
	menu_par4_dot,menu_par5_dot
};

const unsigned char menu_par_ee_addr[]=//TMAX=2
{
	0x00*TMAX,0x01*TMAX,0x04*TMAX,0x0b*TMAX,
	0x12*TMAX,0x18*TMAX
};

const unsigned char menu_item1[][4] =
{
	{'0','-','2','0'},
	{'4','-','2','0'}
};
const unsigned char menu_item2[][4] =
{
	{' ',' ','O','N'},
	{' ','O','F','F'}
};



