
#ifndef	__HT1621__H
#define	__HT1621__H

#define HT1621_CS 		RB7
#define HT1621_RD 		RB6
#define HT1621_WR 		RB5
#define HT1621_DAT 		RB4
#define HT1621_CSEN 	TRISB7
#define HT1621_RDEN 	TRISB6
#define HT1621_WREN 	TRISB5
#define HT1621_DATEN 	TRISB4

#define HT1621_CS_HIGH 	HT1621_CS=1
#define HT1621_CS_LOW 	HT1621_CS=0
#define HT1621_RD_HIGH 	HT1621_RD=1
#define HT1621_RD_LOW 	HT1621_RD=0
#define HT1621_WR_HIGH 	HT1621_WR=1
#define HT1621_WR_LOW 	HT1621_WR=0
#define HT1621_DAT_HIGH HT1621_DAT=1
#define HT1621_DAT_LOW 	HT1621_DAT=0

#define HT1621_CSEN_OUT HT1621_CSEN=0
#define HT1621_CSEN_IN  HT1621_CSEN=1
#define HT1621_RDEN_OUT HT1621_RDEN=0
#define HT1621_RDEN_IN  HT1621_RDEN=1
#define HT1621_WREN_OUT HT1621_WREN=0
#define HT1621_WREN_IN  HT1621_WREN=1
#define HT1621_DATEN_OUT HT1621_DATEN=0
#define HT1621_DATEN_IN  HT1621_DATEN=1

#define LCDON 			0x06
#define LCDOFF 			0x04
#define LCDBIAS122 		0x40
#define LCDBIAS123 		0x48
#define LCDBIAS124 		0x50
#define LCDBIAS132 		0x42
#define LCDBIAS133 		0x4a
#define LCDBIAS134 		0x52
#define LCDSYSEN 		0x02
#define LCDSYSDIS 		0x00
#define LCDWDTDIS 		0x0a
#define LCDWDTEN 		0x0e
#define LCDRC256K 		0x30
#define LCDXTAL32K 		0x28
#define LCDREAD 		0xc0
#define LCDWRITE 		0xa0
#define LCDREAD_WRITE 	0xa0
#define LCDTONE2K		0xc0
#define LCDTONE4K		0x80
#define LCDTONEON		0x12
#define LCDTONEOFF		0x10

#define _Nop()  		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); 
//œÚHT161≥ı ºªØ
extern void Ht1621_Init(void);
extern void Ht1621_WriteCommand(unsigned char CommandWord);
extern void Ht1621_WriteByte(unsigned char pdata,unsigned char cnt);
extern void Ht1621_WriteData(unsigned char Addr,unsigned char *pdata,unsigned char cnt);
extern void Ht1621_CLR(void);
extern void Ht1621_Disp(unsigned char *pdata);
extern void DelayMS(unsigned int iMs);

#endif
