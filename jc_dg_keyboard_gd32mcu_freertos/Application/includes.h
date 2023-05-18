
#ifndef INCLUDES_H
#define INCLUDES_H

//�����ܵ�ͷ�ļ�����

#include <gd32f10x.h>
#include <stdio.h>
//#include <stdarg.h>
#include <limits.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <atomic.h>
#include <event_groups.h>
#include <stream_buffer.h>


#include <string.h>

//���ܿ��ƺ궨�壺

#define BTNS_USE_INT   //����ɨ��ʹ���жϷ�ʽ
//#define HWTD_USE_INT   //Ӳ�����Ź�ʹ���ⲿ�жϣ��½��ص�ƽ����
#define KLEDS_PWM  //�������ȿ���ʹ��pwm��ʽ��2022-10-10

#define HWTD_USE_INT   //Ӳ�����Ź�ʹ���ⲿ�жϣ��½��ص�ƽ����
#define LEDS_FLASH_TASK  //led���Ƶ���˸�ɵ�Ƭ������,��Ϊ��ʱ������
#define SOFT_VERSION    0x05   /* ����汾 2023-05-09 �汾������05 */

#define LEDS_FLASH_TASK  //������˸����

extern const char* g_build_time_str;

#define DEBUG_COM_NUM 0   //���Դ��ں�
#define TOCPU_COM_NUM 1   //��cpuͨ�ŵĴ���


#define UNUSED(x)  

#include "uart.h"
#include "iic.h"
#include "AT9236.h"
#include "main.h"
#include "gd32f103c_eval.h"
#include "gpio.h"

#include "gd32f10x_it.h"
#include "systick.h"
#include "soft_iic_master_app.h"
#include "software_iic_slave.h"

#include "uart_debug_handle.h"
//#include "keys_scan.h"
#include "key_lights_flash.h"



#define ApplicationAddress    0x8005c00
#define PAGE_SIZE             (0x400)    /* 1 Kbyte */
#define DOWN_MD5_OFFET  512    //��������md5ƫ�Ƶ�ַ
#define FILE_MD5_LENGTH 32

#define UPDATE_FLAG_START_ADDR   (ApplicationAddress-PAGE_SIZE)     //������ʼ��ַ��0x805c00



//�������汾2023-01-15
//extern uint8_t GetMcuVersion(void);

extern uint8_t more_debug_info;   //��ӡ����ĵ�����Ϣ��0�򲻴�ӡ�����Դ��ڿ���

//extern SemaphoreHandle_t iic0_has_senddatas;  //iic0��������׼������
extern volatile uint8_t g_iic0_has_senddatas;  //0��ʾû�У�1��ʾ������
#endif

