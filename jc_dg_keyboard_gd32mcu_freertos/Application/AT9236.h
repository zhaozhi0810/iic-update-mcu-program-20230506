/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：AT9236.h
* 摘要：AT9236驱动头文件
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#ifndef __AT9236_H__
#define __AT9236_H__

#include "gd32f10x.h"
//#include "gd32f103c_eval.h"

typedef enum
{
    LED_OFF = 0,
    LED_ON = 0x01,
} LED_control_t;

//设置键灯的亮度pwm【0-100】
void set_keyleds_pwm(uint8_t pwm);/*设置PWM亮度*/
//获取按键灯的亮度值
uint8_t get_keyleds_pwm(void);

void AT9236_LED_lightAll(void);/*点亮全部LED灯*/
void AT9236_LED_control(uint8_t write_address, LED_control_t cmd);/*关闭全部LED灯*/
void AT9236_transmit_byte(uint8_t write_address, uint8_t cmd);/*LED灯单独控制*/
void AT9236_LED_lightAll_OFF(void);/*IIC发送数据至AT9236*/


/*led控制指令解析函数*/
uint8_t cmd_led_control(uint8_t cmd, LED_control_t led_stat);



extern uint8_t g_key_scan_set_num;
extern uint8_t g_GPIO_KEY_map_value[48];
extern uint8_t g_GPIO_LED_map_value[48];/*LED灯映射*/
extern uint8_t g_LED_set_max_num;/*按键灯指令设置最大值*/
#endif
