/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：iic.c
* 摘要：iic初始化配置头文件
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#ifndef __IIC_H__
#define __IIC_H__

#include "gd32f10x.h"
//#include "gd32f103c_eval.h"

#define CH453_IO_PORT      GPIOB
#define CH453_SCL_IO_PIN   GPIO_PIN_8
#define CH453_SDA_IO_PIN   GPIO_PIN_9

/* IIC0*/
#define IIC_SCL_SET      gpio_bit_set(CH453_IO_PORT, CH453_SCL_IO_PIN)  //gpio_set_pin(IIC_SCL_PIN)   //SCL value is 1
#define IIC_SCL_RESET    gpio_bit_reset(CH453_IO_PORT, CH453_SCL_IO_PIN)//gpio_reset_pin(IIC_SCL_PIN) //SCL value is 0
#define IIC_SDA_SET     gpio_bit_set(CH453_IO_PORT, CH453_SDA_IO_PIN) //gpio_set_pin(IIC_SDA_PIN)   //SDA value is 1
#define IIC_SDA_RESET   gpio_bit_reset(CH453_IO_PORT, CH453_SDA_IO_PIN) //gpio_reset_pin(IIC_SDA_PIN) //SDA value is 0
#define READ_SDA   gpio_input_bit_get(CH453_IO_PORT, CH453_SDA_IO_PIN)//gpio_read_input_pin_value(IIC_SDA_PIN)  //SDA 
#define SDA_IN()  {gpio_init(CH453_IO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, CH453_SDA_IO_PIN); }
#define SDA_OUT() {gpio_init(CH453_IO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CH453_SDA_IO_PIN);}
#define     CH453_I2C_ADDR      0x40
#define     CH453_I2C_MASK      0x3E

//void IIC_init(void);/*iic初始化*/
void CH453_Write(uint16_t cmd); /*写命令*/


/*iic初始化*/
void IIC0_init(void);
/*iic初始化*/
void IIC1_init(void);


/*任务5  把iic缓存的东西发出去
*/
void iic0_send_data_task(void *pdata);

#endif
