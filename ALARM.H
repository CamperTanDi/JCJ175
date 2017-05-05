extern void Alarm_Init(void);
extern void Alarm_Handle(void);
extern volatile unsigned char keyflagalalrm;

#define THA RA3
#define TLA RA2
#define HHA RA1
#define HLA RA0
#define THAEN TRISA3
#define TLAEN TRISA2
#define HHAEN TRISA1
#define HLAEN TRISA0
#define THAEN_OUT THAEN=0
#define TLAEN_OUT TLAEN=0
#define HHAEN_OUT HHAEN=0
#define HLAEN_OUT HLAEN=0




extern volatile unsigned char alarmflag;

#define ALARM RA4
#define LIGHT RA5

#define ALARM_EN TRISA4
#define LIGHT_EN TRISA5


#define ALARM_OUT ALARM_EN=0
#define LIGHT_OUT LIGHT_EN=0 


#define LIGHTON   LIGHT=1
#define LIGHTOFF  LIGHT=0
#define ALARMON   ALARM=1
#define ALARMOFF  ALARM=0