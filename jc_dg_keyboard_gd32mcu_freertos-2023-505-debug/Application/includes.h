
#ifndef INCLUDES_H
#define INCLUDES_H

//用于总的头文件包含

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

//功能控制宏定义：

#define BTNS_USE_INT   //按键扫描使用中断方式
//#define HWTD_USE_INT   //硬件看门狗使用外部中断，下降沿电平触发
#define KLEDS_PWM  //键灯亮度控制使用pwm方式，2022-10-10

#define HWTD_USE_INT   //硬件看门狗使用外部中断，下降沿电平触发
#define LEDS_FLASH_TASK  //led键灯的闪烁由单片机控制,改为定时器控制
#define SOFT_VERSION    0x04   /* 软件版本 2023-04-06 版本升级到06 */

#define LEDS_FLASH_TASK  //键灯闪烁控制

extern const char* g_build_time_str;

#define DEBUG_COM_NUM 0   //调试串口号
#define TOCPU_COM_NUM 1   //与cpu通信的串口


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

//获得软件版本2023-01-15
extern uint8_t GetMcuVersion(void);

extern uint8_t more_debug_info;   //打印更多的调试信息，0则不打印，调试串口控制

//extern SemaphoreHandle_t iic0_has_senddatas;  //iic0发送数据准备好了
extern volatile uint8_t g_iic0_has_senddatas;  //0表示没有，1表示有数据
#endif

