/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：main.h
* 摘要：主程序头文件
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include "gd32f10x.h"
//#include "gd32f103c_eval.h"
#include <FreeRTOS.h>
#include <task.h>

/* The circle buffer function defined by warren */
#define CIRC_SIZE   64      /* serial frame circle buffer size 2023-0310 128 ---> 64*/
/* serial frame circle buffer initialize  */
#define CIRC_RELEASE(cb)        (cb.tail = cb.head = 0)
/* Return count in buffer.  */
#define CIRC_RM_CNT(cb)     ((CIRC_SIZE + cb.head - cb.tail) % CIRC_SIZE)
/* Put one word to circle buffer.  */
#define CIRC_PUT_CH(cb, val)    {cb.buf[cb.head] = val; cb.head = (cb.head + 1) % CIRC_SIZE;}
/* Get one word from circle buffer.  */
#define CIRC_GET_CH(cb, val)    {val = cb.buf[cb.tail]; cb.tail = (cb.tail + 1) % CIRC_SIZE;}

#define NUM_OF_KEY       33
#define CMD_READ_PRESSKEY   0x01
#define CMD_READ_RELEASEKEY 0x02
#define CMD_READ_LIGHT      0x10
#define CMD_WRITE_LIGHT     0x20
#define CMD_DEVICE_TYPE     0x30
#define CMD_SOFT_VERSION    0x40
#define CMD_RESET           0x50
#define CMD_LIGHT_LED       0x60
#define CMD_LIGHT_OUT       0x70
#define CMD_LIGHT_FLASH     0x80   //注意与jc_keyboard对应
#define CMD_UPDATE_MCU      0x90   //单片机通过3399的iic升级，2023-05-05


typedef struct _IIC_circ_buf
{
    uint8_t head;
    uint8_t tail;
    uint8_t buf[CIRC_SIZE];
} IIC_CircleBuffer;

typedef struct
{
    uint8_t CH453_keyValue;
    uint8_t AT9236_reg;
    uint8_t REPORT_keyValue;
    uint8_t KEY_press;
} CH453_AT9236_map_t;

typedef struct
{
    uint8_t multikey_mark;
    uint8_t key_value[3];
    uint8_t key_type;
    uint8_t send_mark;
} IIC0_GPIO_upload_t;



extern TaskHandle_t  TaskHandle_ToCpu_IIC;   //存放IIC任务指针
extern TaskHandle_t  TaskHandle_IIC0_SendData;  //iic0发送数据

extern IIC_CircleBuffer g_i2c0_rxbuf;
extern IIC_CircleBuffer g_i2c0_txbuf;
extern uint8_t g_IIC_tx_buf[7];
//iic0是否空闲
extern volatile uint8_t g_iic0_is_Idle;  //0表示非空闲，1表示空闲



//键灯控制任务，调试串口控制使用
extern void key_light_control_debug_task(void *pdata);

/*填充要发送的数据*/
extern void fill_tx_data(uint8_t txv2, uint8_t txv3, uint8_t txv4, uint8_t txv5);

extern uint8_t CheckSum(const uint8_t *buf, uint8_t len);

/*按键值对应寄存器*/
extern void CH453_AT9236_map_init(void);


#endif
