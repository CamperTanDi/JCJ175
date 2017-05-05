#define TMAX 2
#define CH_MAX 2
#define MS_1000	1000
#define MS_20  30//轻按下键时间
//#define MS_60  30
//#define MS_150 900
#define MS_500 300//连续按键500ms
#define MS_2S  2000//连续按键2s
#define MS_1_5S  1500//连续按键2s
#define MS_EXITPAR 10*MS_2S   //退出菜单时间  12s
#define MS_INIT  MS_2S/2 //初始界面时间  1s
#define KEYCAP  180
#define PVADDR 0xf0


#include "ad.h"
#include "i2_rw.h"
#include "uart_0.h"
#include "code.h"
#include "int.h"
#include "led.h"
#include "bs.h"
#include "eeprom_routines.h"
#include "math.h"
#include "ht1621.h"
#include "sht10.h"
#include "pic.h"
#include "main.h"
#include "menu.h"
#include "key.h"
#include "alarm.h"





#define parconfigindex 1
#define parbiansongindex 4
#define parhumiindex 3
#define humihidecount 2
#define collectcount 5

typedef struct {
	int sc;
	int bsl;
	int bsh;
	int ha;
	int yh;
	int la;
	int yl;
	int bslvalue;
	int bshvalue;
}_parItem;
extern volatile _parItem parItem[CH_MAX];
typedef struct {
	int addr;
	int baud;
	int spe;
	int jzflag;
	int outflag;
}_configItem;
extern volatile _configItem configItem;