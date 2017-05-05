#ifndef _UART_0
#define _UART_0

#define GETMAX 			8
#define READ 			3
#define READONLY 		4
#define WRITE 			6
#define DATAMAX 		4
#define ADDR_ADDR 		0
#define BAUD_ADDR 		1

#define SDI 			RC5
#define SDIEN			TRISC5
#define SDI_RECEIVE		SDI=0
#define SDI_SEND		SDI=1
#define SDIEN_OUT 		SDIEN=0
#define SDIEN_IN		SDIEN=1
#define ADDR_PAR_COUNT	0x64

extern unsigned int modbusCRC(unsigned char *daBuf,unsigned char len);
extern unsigned char juscanCRC(unsigned char *daBuf,unsigned char len);
extern void Uart0_Init(void);
extern void Uart0_Handle(void);

extern bank2 volatile unsigned char uartinitflag;
extern bank2 volatile unsigned char getover;
extern bank2 volatile unsigned char rxtimer;
extern bank2 volatile unsigned char sendcount;
extern bank2 volatile unsigned char getcount;
extern bank2 volatile unsigned char sendmax;
extern bank2 volatile unsigned char waittime;
extern bank3 volatile unsigned char getdata[];
extern bank3 volatile unsigned char senddata[];
extern const int baudcode[];
#endif