

#ifndef __KEYS_SCAN_H__
#define __KEYS_SCAN_H__

#include "gd32f10x.h"

#define KEY_MAX 36   //����������


//#define KEY_BYTES_RECORD (KEY_MAX/8+(!!(KEY_MAX%8)))


typedef struct btn_info{
	uint8_t  value[KEY_MAX];	     //0��ʾ�ɿ���1��ʾ����
	uint8_t  reportEn[KEY_MAX];   //1����Ҫ�ϱ���0����Ҫ�ϱ�
	uint16_t  pressCnt[KEY_MAX];     //��������������
}BTN_INFO;



extern uint8_t g_key_scan_set_num;/*ɨ�谴����*/

//�������ų�ʼ��
void keys_scan_init(void);

void keys_scan_task(void);
#endif


