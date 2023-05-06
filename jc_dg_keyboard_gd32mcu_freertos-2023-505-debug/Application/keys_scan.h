

#ifndef __KEYS_SCAN_H__
#define __KEYS_SCAN_H__

#include "gd32f10x.h"

#define KEY_MAX 36   //键盘最大的数


//#define KEY_BYTES_RECORD (KEY_MAX/8+(!!(KEY_MAX%8)))


typedef struct btn_info{
	uint8_t  value[KEY_MAX];	     //0表示松开，1表示按下
	uint8_t  reportEn[KEY_MAX];   //1，需要上报，0不需要上报
	uint16_t  pressCnt[KEY_MAX];     //消抖，长按区分
}BTN_INFO;



extern uint8_t g_key_scan_set_num;/*扫描按键数*/

//按键引脚初始化
void keys_scan_init(void);

void keys_scan_task(void);
#endif


