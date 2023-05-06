/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：gpio.h
* 摘要：gpio初始化配置头文件
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#ifndef __GPIO_H__
#define __GPIO_H__

#include "gd32f10x.h"
//#include "gd32f103c_eval.h"

typedef enum
{
	RGB_ALL_OFF = 0,
    RGB_R = 0x28,
    RGB_G = 0x29,
    RGB_B = 0x2A,
	RGB_ALL = 0x2B   //2023-02-22  added by dazhi
} RGB_color_t;

void gd32_gpio_init(void);/*系统IO初始化*/
void RGB_control(RGB_color_t color);/*RGB GPIO 控制*/
void RGB_control_off(RGB_color_t color);/*RGB GPIO 控制*/
void LED_control(uint8_t ctrlCode);/*LED控制*/

#endif
