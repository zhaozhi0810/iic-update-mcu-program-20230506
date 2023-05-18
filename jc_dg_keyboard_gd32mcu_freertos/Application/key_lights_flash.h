

#ifndef __KEY_LIGHTS_FLASH_H__
#define __KEY_LIGHTS_FLASH_H__

#include "gd32f10x.h"
#include "at9236.h"


//led��˸��ʼ��
void key_light_leds_flash_init(void);

//����ĳ��led����˸
/*
	whichled :��ʾ�ĸ�����˸��[1-43]  ,ʵ����ЧֵΪ0-42�������м�������			 
	flash_freq :��ʾ��˸������,ֻ����0-3���ֱ��ʾ500ms��800ms��1000ms��2000ms	 
*/
void light_leds_add_flash(uint8_t whichled, uint8_t flash_freq);



//����ӿ�������˸�ͳ�������ƣ���ֹͣ��˸����
//whichled: 0-42 ,����42��ʾ����led
//status : 0��ʾϨ�𣬷����ʾ����
void key_light_leds_control2(uint8_t whichled,uint8_t status);
#endif
