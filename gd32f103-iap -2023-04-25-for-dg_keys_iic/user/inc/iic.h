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




/* The circle buffer function defined by warren */
#define CIRC_SIZE   1032      /* serial frame circle buffer size 2023-0310 128 ---> 1032*/
/* serial frame circle buffer initialize  */
#define CIRC_RELEASE(cb)        (cb.tail = cb.head = 0)
/* Return count in buffer.  */
#define CIRC_RM_CNT(cb)     ((CIRC_SIZE + cb.head - cb.tail) % CIRC_SIZE)
/* Put one word to circle buffer.  */
#define CIRC_PUT_CH(cb, val)    {cb.buf[cb.head] = val; cb.head = (cb.head + 1) % CIRC_SIZE;}
/* Get one word from circle buffer.  */
#define CIRC_GET_CH(cb, val)    {val = cb.buf[cb.tail]; cb.tail = (cb.tail + 1) % CIRC_SIZE;}



#define CMD_UPDATE_MCU      0x90   //单片机通过3399的iic升级，2023-05-05


typedef struct _IIC_circ_buf
{
    uint16_t head;
    uint16_t tail;
    uint8_t buf[CIRC_SIZE];
} IIC_CircleBuffer;

extern IIC_CircleBuffer g_i2c0_rxbuf;/*I2C接收缓冲区*/
extern IIC_CircleBuffer g_i2c0_txbuf;/*I2C发送缓冲区*/
extern uint8_t g_IIC_tx_buf[7];   /*I2C发送缓存*/

//void IIC_init(void);/*iic初始化*/
void CH453_Write(uint16_t cmd); /*写命令*/


/*iic初始化*/
void IIC0_init(void);
/*iic初始化*/
void IIC1_init(void);


/*任务5  把iic缓存的东西发出去
*/
//void iic0_send_data_task(void *pdata);
void iic0_send_data(uint8_t *buf,uint8_t len);

//正常返回0，否则返回-1
int32_t i2c0_Receive(uint8_t *rdata,uint16_t len,uint32_t timeout);

//获取一个字符
uint32_t i2c0_Receive_byte(uint8_t *key);

//iic只发送一个字节，
uint32_t Send_Byte_to_iic0 (uint8_t c);


#endif
