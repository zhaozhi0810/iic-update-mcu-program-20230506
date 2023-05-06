

#ifndef __KEY_LIGHTS_FLASH_H__
#define __KEY_LIGHTS_FLASH_H__

#include "gd32f10x.h"
#include "at9236.h"


//led闪烁初始化
void key_light_leds_flash_init(void);

//增加某个led灯闪烁
/*
	whichled :表示哪个灯闪烁，[1-43]  ,实际有效值为0-42，函数中间做调整			 
	flash_freq :表示闪烁的速率,只能是0-3，分别表示500ms，800ms，1000ms，2000ms	 
*/
void light_leds_add_flash(uint8_t whichled, uint8_t flash_freq);



//这个接口用于闪烁和常亮灭控制，会停止闪烁控制
//whichled: 0-42 ,其中42表示所有led
//status : 0表示熄灭，非零表示点亮
void key_light_leds_control2(uint8_t whichled,uint8_t status);
#endif
