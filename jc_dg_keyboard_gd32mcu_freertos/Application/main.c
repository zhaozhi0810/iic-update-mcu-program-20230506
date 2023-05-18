/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：main.c
* 摘要：主程序
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#include "includes.h"


#define KB_NO_NEW_KEY    0xff    /* 无按键按下时的值 */
#define KB_DEBOUNCE_THRES      20  /* 消抖阈值 */

const char* g_build_time_str = "Buildtime :"__DATE__" "__TIME__;   //获得编译时间


//OS_CPU_SR cpu_sr = 0;/*UCOSII定义，不加g_*/
TaskHandle_t  TaskHandle_ToCpu_IIC;   //存放IIC任务指针
TaskHandle_t  TaskHandle_Light_Control;
TaskHandle_t  TaskHandle_IIC0_SendData;  //iic0发送数据
//SemaphoreHandle_t write_iic0_txbuf_bin;  //写缓存互斥量
//SemaphoreHandle_t iic0_has_senddatas;  //iic0发送数据准备好了



uint8_t g_key_scan_set_num = 0;/*扫描按键数*/
uint8_t g_card_device_type = 0xff;/*按键类型*/
uint8_t g_LED_set_max_num = 0;/*按键灯指令设置最大值*/
//uint8_t g_IIC_tx_data[7] = {0x55, 0xaa};  /*I2C发送缓存*/
uint8_t g_IIC_tx_buf[36] = {0x55, 0xaa};   /*I2C发送缓存*/

IIC_CircleBuffer g_i2c0_rxbuf;/*I2C接收缓冲区*/
IIC_CircleBuffer g_i2c0_txbuf;/*I2C发送缓冲区*/

//CH453_AT9236_map_t g_CH453_AT9236_map[33];/*CH453寄存器映射*/

uint8_t g_GPIO_KEY_map_value[48];/*按键映射*/   
uint8_t g_GPIO_LED_map_value[48];/*LED灯映射*/

uint8_t g_save_presskey1 = 0;
uint8_t g_save_releasekey1 = 0;

extern volatile uint8_t g_iic0_send_size;  //默认是发送7个字节
//uint8_t g_pwm_value = 0;/*PWM亮度,0-250*/
//uint8_t g_press_down_multi_num[10];/*多按键*/



#ifdef USE_CH453_INT
int G_CH453_INT_FLAG;
#endif



#if 0
/*按键值对应寄存器*/
void CH453_AT9236_map_init(void)
{
    int i = 0;
    g_CH453_AT9236_map[0].CH453_keyValue = 0x72;  // 6
    g_CH453_AT9236_map[0].AT9236_reg = 0x26;
    g_CH453_AT9236_map[0].REPORT_keyValue = 0x13;
    g_CH453_AT9236_map[1].CH453_keyValue = 0x52;  // 9
    g_CH453_AT9236_map[1].AT9236_reg = 0x27;
    g_CH453_AT9236_map[1].REPORT_keyValue = 0x16;
    g_CH453_AT9236_map[2].CH453_keyValue = 0x42;  // #
    g_CH453_AT9236_map[2].AT9236_reg = 0x28;
    g_CH453_AT9236_map[2].REPORT_keyValue = 0x18;
    g_CH453_AT9236_map[3].CH453_keyValue = 0x5A;  // 0
    g_CH453_AT9236_map[3].AT9236_reg = 0x29;
    g_CH453_AT9236_map[3].REPORT_keyValue = 0x0D;
    g_CH453_AT9236_map[4].CH453_keyValue = 0x58;  // 8
    g_CH453_AT9236_map[4].AT9236_reg = 0x2A;
    g_CH453_AT9236_map[4].REPORT_keyValue = 0x15;
    g_CH453_AT9236_map[5].CH453_keyValue = 0x70;  // 5
    g_CH453_AT9236_map[5].AT9236_reg = 0x2B;
    g_CH453_AT9236_map[5].REPORT_keyValue = 0x12;
    g_CH453_AT9236_map[6].CH453_keyValue = 0x50;  // *
    g_CH453_AT9236_map[6].AT9236_reg = 0x2C;
    g_CH453_AT9236_map[6].REPORT_keyValue = 0x17;
    g_CH453_AT9236_map[7].CH453_keyValue = 0x60;  // 7
    g_CH453_AT9236_map[7].AT9236_reg = 0x2D;
    g_CH453_AT9236_map[7].REPORT_keyValue = 0x14;
    g_CH453_AT9236_map[8].CH453_keyValue = 0x48;  // 4
    g_CH453_AT9236_map[8].AT9236_reg = 0x2E;
    g_CH453_AT9236_map[8].REPORT_keyValue = 0x11;
    g_CH453_AT9236_map[9].CH453_keyValue = 0x40;  // 1
    g_CH453_AT9236_map[9].AT9236_reg = 0x2F;
    g_CH453_AT9236_map[9].REPORT_keyValue = 0x0E;
    g_CH453_AT9236_map[10].CH453_keyValue = 0x68;  // 2
    g_CH453_AT9236_map[10].AT9236_reg = 0x30;
    g_CH453_AT9236_map[10].REPORT_keyValue = 0x0F;
    g_CH453_AT9236_map[11].CH453_keyValue = 0x5B;  // 图示6
    g_CH453_AT9236_map[11].AT9236_reg = 0x31;
    g_CH453_AT9236_map[11].REPORT_keyValue = 0x06;
    g_CH453_AT9236_map[12].CH453_keyValue = 0x6B;  // 图示5
    g_CH453_AT9236_map[12].AT9236_reg = 0x32;
    g_CH453_AT9236_map[12].REPORT_keyValue = 0x05;
    g_CH453_AT9236_map[13].CH453_keyValue = 0x73;  // 图示4
    g_CH453_AT9236_map[13].AT9236_reg = 0x33;
    g_CH453_AT9236_map[13].REPORT_keyValue = 0x04;
    g_CH453_AT9236_map[14].CH453_keyValue = 0x53;  // 图示3
    g_CH453_AT9236_map[14].AT9236_reg = 0x34;
    g_CH453_AT9236_map[14].REPORT_keyValue = 0x03;
    g_CH453_AT9236_map[15].CH453_keyValue = 0x63;  // 图示2
    g_CH453_AT9236_map[15].AT9236_reg = 0x35;
    g_CH453_AT9236_map[15].REPORT_keyValue = 0x02;
    g_CH453_AT9236_map[16].CH453_keyValue = 0x4B;  // 图示1
    g_CH453_AT9236_map[16].AT9236_reg = 0x36;
    g_CH453_AT9236_map[16].REPORT_keyValue = 0x01;
    g_CH453_AT9236_map[17].CH453_keyValue = 0x43;  // 内通
    g_CH453_AT9236_map[17].AT9236_reg = 0x37;
    g_CH453_AT9236_map[17].REPORT_keyValue = 0x1D;
    g_CH453_AT9236_map[18].CH453_keyValue = 0x41;  // 外通
    g_CH453_AT9236_map[18].AT9236_reg = 0x38;
    g_CH453_AT9236_map[18].REPORT_keyValue = 0x1E;
    g_CH453_AT9236_map[19].CH453_keyValue = 0x49;  // 图示7
    g_CH453_AT9236_map[19].AT9236_reg = 0x39;
    g_CH453_AT9236_map[19].REPORT_keyValue = 0x07;
    g_CH453_AT9236_map[20].CH453_keyValue = 0x61;  // 图示8
    g_CH453_AT9236_map[20].AT9236_reg = 0x3A;
    g_CH453_AT9236_map[20].REPORT_keyValue = 0x08;
    g_CH453_AT9236_map[21].CH453_keyValue = 0x51;  // 图示9
    g_CH453_AT9236_map[21].AT9236_reg = 0x3B;
    g_CH453_AT9236_map[21].REPORT_keyValue = 0x09;
    g_CH453_AT9236_map[22].CH453_keyValue = 0x71;  // 图示10
    g_CH453_AT9236_map[22].AT9236_reg = 0x3C;
    g_CH453_AT9236_map[22].REPORT_keyValue = 0x0A;
    g_CH453_AT9236_map[23].CH453_keyValue = 0x69;  // 图示11
    g_CH453_AT9236_map[23].AT9236_reg = 0x3D;
    g_CH453_AT9236_map[23].REPORT_keyValue = 0x0B;
    g_CH453_AT9236_map[24].CH453_keyValue = 0x59;  // 图示12
    g_CH453_AT9236_map[24].AT9236_reg = 0x3E;
    g_CH453_AT9236_map[24].REPORT_keyValue = 0x0C;
    g_CH453_AT9236_map[25].CH453_keyValue = 0x62;  // 3
    g_CH453_AT9236_map[25].AT9236_reg = 0x3F;
    g_CH453_AT9236_map[25].REPORT_keyValue = 0x10;
    g_CH453_AT9236_map[26].CH453_keyValue = 0x6A;  // 电话
    g_CH453_AT9236_map[26].AT9236_reg = 0x40;
    g_CH453_AT9236_map[26].REPORT_keyValue = 0x19;
    g_CH453_AT9236_map[27].CH453_keyValue = 0x64;  // 上
    g_CH453_AT9236_map[27].AT9236_reg = 0x41;
    g_CH453_AT9236_map[27].REPORT_keyValue = 0x1F;
    g_CH453_AT9236_map[28].CH453_keyValue = 0x4A;  // 音量+
    g_CH453_AT9236_map[28].AT9236_reg = 0x42;
    g_CH453_AT9236_map[28].REPORT_keyValue = 0x1A;
    g_CH453_AT9236_map[29].CH453_keyValue = 0x54;  // 下
    g_CH453_AT9236_map[29].AT9236_reg = 0x43;
    g_CH453_AT9236_map[29].REPORT_keyValue = 0x20;
    g_CH453_AT9236_map[30].CH453_keyValue = 0x4C;  // 音量-
    g_CH453_AT9236_map[30].AT9236_reg = 0x44;
    g_CH453_AT9236_map[30].REPORT_keyValue = 0x1B;
    g_CH453_AT9236_map[31].CH453_keyValue = 0x5C;  // PTT
    g_CH453_AT9236_map[31].AT9236_reg = 0x45;
    g_CH453_AT9236_map[31].REPORT_keyValue = 0x21;
    g_CH453_AT9236_map[32].CH453_keyValue = 0x44;  // 切换
    g_CH453_AT9236_map[32].AT9236_reg = 0x46;
    g_CH453_AT9236_map[32].REPORT_keyValue = 0x1C;
    for (i = 0; i < NUM_OF_KEY; i++)
    {
        g_CH453_AT9236_map[i].KEY_press = 0;
    }
}
#endif


/*
IO采集对应的按键报告值
A:对应的按键板型号为：嵌1、嵌2、壁挂2、防风雨
*/
void GPIO_report_num_map_init_A(void)
{
//    int i;
    g_GPIO_KEY_map_value[0] = 0x1d;//内通
    g_GPIO_KEY_map_value[1] = 0x01;//图示1
    g_GPIO_KEY_map_value[2] = 0x02;//图示2
    g_GPIO_KEY_map_value[3] = 0x03;//图示3
    g_GPIO_KEY_map_value[4] = 0x04;//图示4
    g_GPIO_KEY_map_value[5] = 0x05;//图示5
    g_GPIO_KEY_map_value[6] = 0x06;//图示6
    g_GPIO_KEY_map_value[7] = 0x0E;//1
    g_GPIO_KEY_map_value[8] = 0x11;//4
    g_GPIO_KEY_map_value[9] = 0x14;//7
    g_GPIO_KEY_map_value[10] = 0x17;//*
    g_GPIO_KEY_map_value[11] = 0x0D;//0
    g_GPIO_KEY_map_value[12] = 0x15;//8
    g_GPIO_KEY_map_value[13] = 0x12;//5
    g_GPIO_KEY_map_value[14] = 0x0F;//2
    g_GPIO_KEY_map_value[15] = 0x10;//3
    g_GPIO_KEY_map_value[16] = 0x13;//6
    g_GPIO_KEY_map_value[17] = 0x16;//9
    g_GPIO_KEY_map_value[18] = 0x18;//#
    g_GPIO_KEY_map_value[19] = 0x19;//电话 拨号
    g_GPIO_KEY_map_value[20] = 0x27;//测试
    g_GPIO_KEY_map_value[21] = 0x1A;//音量+
    g_GPIO_KEY_map_value[22] = 0x1B;//音量-
    g_GPIO_KEY_map_value[23] = 0x21;//PPT
    g_GPIO_KEY_map_value[24] = 0x20;//下
    g_GPIO_KEY_map_value[25] = 0x1F;//上
    g_GPIO_KEY_map_value[26] = 0x0C;//图示12
    g_GPIO_KEY_map_value[27] = 0x0B;//图示11
    g_GPIO_KEY_map_value[28] = 0x0A;//图示10
    g_GPIO_KEY_map_value[29] = 0x09;//图示9
    g_GPIO_KEY_map_value[30] = 0x08;//图示8
    g_GPIO_KEY_map_value[31] = 0x07;//图示7
    g_GPIO_KEY_map_value[32] = 0x1E;//外通
    g_GPIO_KEY_map_value[33] = 0x22;//左
    g_GPIO_KEY_map_value[34] = 0x23;//右
    g_GPIO_KEY_map_value[35] = 0x24;//ok
    g_GPIO_LED_map_value[0] = 0x26;//内通
    g_GPIO_LED_map_value[1] = 0x27;//图示1
    g_GPIO_LED_map_value[2] = 0x28;//图示2
    g_GPIO_LED_map_value[3] = 0x29;//图示3
    g_GPIO_LED_map_value[4] = 0x2A;//图示4
    g_GPIO_LED_map_value[5] = 0x2B;//图示5
    g_GPIO_LED_map_value[6] = 0x2C;//图示6
    g_GPIO_LED_map_value[7] = 0x34;//1
    g_GPIO_LED_map_value[8] = 0x46;//4
    g_GPIO_LED_map_value[9] = 0x41;//7
    g_GPIO_LED_map_value[10] = 0x3C;//*
    g_GPIO_LED_map_value[11] = 0x3B;//0
    g_GPIO_LED_map_value[12] = 0x40;//8
    g_GPIO_LED_map_value[13] = 0x45;//5
    g_GPIO_LED_map_value[14] = 0x35;//2
    g_GPIO_LED_map_value[15] = 0x36;//3
    g_GPIO_LED_map_value[16] = 0x44;//6
    g_GPIO_LED_map_value[17] = 0x3F;//9
    g_GPIO_LED_map_value[18] = 0x3A;//#
    g_GPIO_LED_map_value[19] = 0x37;//电话 拨号
    g_GPIO_LED_map_value[20] = 0x43;//测试
    g_GPIO_LED_map_value[21] = 0x3E;//音量+
    g_GPIO_LED_map_value[22] = 0x39;//音量-
    g_GPIO_LED_map_value[23] = 0x38;//PPT
    g_GPIO_LED_map_value[24] = 0x3D;//下
    g_GPIO_LED_map_value[25] = 0x42;//上
    g_GPIO_LED_map_value[26] = 0x33;//图示12
    g_GPIO_LED_map_value[27] = 0x32;//图示11
    g_GPIO_LED_map_value[28] = 0x31;//图示10
    g_GPIO_LED_map_value[29] = 0x30;//图示9
    g_GPIO_LED_map_value[30] = 0x2F;//图示8
    g_GPIO_LED_map_value[31] = 0x2E;//图示7
    g_GPIO_LED_map_value[32] = 0x2D;//外通
    g_GPIO_LED_map_value[33] = 0x26;//左
    g_GPIO_LED_map_value[34] = 0x26;//右
    g_GPIO_LED_map_value[35] = 0x26;//ok
//    for (i = 0; i < 10; i++)
//    {
//        g_press_down_multi_num[i] = 0xff;
//    }
}


/*
IO采集对应的按键报告值
B:对应的按键板型号为：嵌3
*/
void GPIO_report_num_map_init_B(void)
{
//    int i;
//	printf("GPIO_report_num_map_init_B \r\n");
    g_GPIO_KEY_map_value[0] = 0x1d;//内通
    g_GPIO_KEY_map_value[1] = 0x01;//图示1
    g_GPIO_KEY_map_value[2] = 0x02;//图示2
    g_GPIO_KEY_map_value[3] = 0x03;//图示3
    g_GPIO_KEY_map_value[4] = 0x04;//图示4
    g_GPIO_KEY_map_value[5] = 0x05;//图示5
    g_GPIO_KEY_map_value[6] = 0x06;//图示6
    g_GPIO_KEY_map_value[7] = 0x0E;//1
    g_GPIO_KEY_map_value[8] = 0x11;//4
    g_GPIO_KEY_map_value[9] = 0x14;//7
    g_GPIO_KEY_map_value[10] = 0x17;//*
    g_GPIO_KEY_map_value[11] = 0x0D;//0
    g_GPIO_KEY_map_value[12] = 0x15;//8
    g_GPIO_KEY_map_value[13] = 0x12;//5
    g_GPIO_KEY_map_value[14] = 0x0F;//2
    g_GPIO_KEY_map_value[15] = 0x10;//3
    g_GPIO_KEY_map_value[16] = 0x13;//6
    g_GPIO_KEY_map_value[17] = 0x16;//9
    g_GPIO_KEY_map_value[18] = 0x18;//#
    g_GPIO_KEY_map_value[19] = 0x19;//电话 拨号
    g_GPIO_KEY_map_value[20] = 0x27;//测试
    g_GPIO_KEY_map_value[21] = 0x1A;//音量+
    g_GPIO_KEY_map_value[22] = 0x1B;//音量-
    g_GPIO_KEY_map_value[23] = 0x21;//PPT
    g_GPIO_KEY_map_value[24] = 0x20;//下
    g_GPIO_KEY_map_value[25] = 0x1F;//上
    g_GPIO_KEY_map_value[26] = 0x0C;//图示12
    g_GPIO_KEY_map_value[27] = 0x0B;//图示11
    g_GPIO_KEY_map_value[28] = 0x0A;//图示10
    g_GPIO_KEY_map_value[29] = 0x09;//图示9
    g_GPIO_KEY_map_value[30] = 0x08;//图示8
    g_GPIO_KEY_map_value[31] = 0x07;//图示7
    g_GPIO_KEY_map_value[32] = 0x1E;//外通
    g_GPIO_KEY_map_value[33] = 0x22;//左
    g_GPIO_KEY_map_value[34] = 0x23;//右
    g_GPIO_KEY_map_value[35] = 0x24;//ok
    g_GPIO_LED_map_value[0] = 0x26;//内通
    g_GPIO_LED_map_value[1] = 0x27;//图示1
    g_GPIO_LED_map_value[2] = 0x28;//图示2
    g_GPIO_LED_map_value[3] = 0x29;//图示3
    g_GPIO_LED_map_value[4] = 0x2A;//图示4
    g_GPIO_LED_map_value[5] = 0x2B;//图示5
    g_GPIO_LED_map_value[6] = 0x2C;//图示6
    g_GPIO_LED_map_value[7] = 0x2d;//1
    g_GPIO_LED_map_value[8] = 0x31;//4  
    g_GPIO_LED_map_value[9] = 0x36;//7  
    g_GPIO_LED_map_value[10] = 0x3C;//*
    g_GPIO_LED_map_value[11] = 0x3B;//0
    g_GPIO_LED_map_value[12] = 0x37;//8  
    g_GPIO_LED_map_value[13] = 0x32;//5  
    g_GPIO_LED_map_value[14] = 0x2E;//2  
    g_GPIO_LED_map_value[15] = 0x2F;//3  
    g_GPIO_LED_map_value[16] = 0x33;//6  
    g_GPIO_LED_map_value[17] = 0x3F;//9
    g_GPIO_LED_map_value[18] = 0x3A;//#
    g_GPIO_LED_map_value[19] = 0x30;//电话 拨号  
    g_GPIO_LED_map_value[20] = 0x34;//测试  
    g_GPIO_LED_map_value[21] = 0x3E;//音量+
    g_GPIO_LED_map_value[22] = 0x39;//音量-
    g_GPIO_LED_map_value[23] = 0x38;//PPT
    g_GPIO_LED_map_value[24] = 0x3D;//下
    g_GPIO_LED_map_value[25] = 0x35;//上  
    g_GPIO_LED_map_value[26] = 0x40;//图示12  
    g_GPIO_LED_map_value[27] = 0x41;//图示11  
    g_GPIO_LED_map_value[28] = 0x42;//图示10  
    g_GPIO_LED_map_value[29] = 0x43;//图示9  
    g_GPIO_LED_map_value[30] = 0x44;//图示8  
    g_GPIO_LED_map_value[31] = 0x45;//图示7  
    g_GPIO_LED_map_value[32] = 0x46;//外通  
    g_GPIO_LED_map_value[33] = 0x26;//左
    g_GPIO_LED_map_value[34] = 0x26;//右
    g_GPIO_LED_map_value[35] = 0x26;//ok
//    for (i = 0; i < 10; i++)
//    {
//        g_press_down_multi_num[i] = 0xff;
//    }
}


/*
IO采集对应的按键报告值
C:对应的按键板型号为：多功能
*/
void GPIO_report_num_map_init_C(void)
{
//    int i;
    g_GPIO_KEY_map_value[0] = 0x1d;//内通
    g_GPIO_KEY_map_value[1] = 0x01;//图示1
    g_GPIO_KEY_map_value[2] = 0x02;//图示2
    g_GPIO_KEY_map_value[3] = 0x03;//图示3
    g_GPIO_KEY_map_value[4] = 0x04;//图示4
    g_GPIO_KEY_map_value[5] = 0x05;//图示5
    g_GPIO_KEY_map_value[6] = 0x06;//图示6
    g_GPIO_KEY_map_value[7] = 0x0E;//1
    g_GPIO_KEY_map_value[8] = 0x11;//4
    g_GPIO_KEY_map_value[9] = 0x14;//7
    g_GPIO_KEY_map_value[10] = 0x17;//*
    g_GPIO_KEY_map_value[11] = 0x0D;//0
    g_GPIO_KEY_map_value[12] = 0x15;//8
    g_GPIO_KEY_map_value[13] = 0x12;//5
    g_GPIO_KEY_map_value[14] = 0x0F;//2
    g_GPIO_KEY_map_value[15] = 0x10;//3
    g_GPIO_KEY_map_value[16] = 0x13;//6
    g_GPIO_KEY_map_value[17] = 0x16;//9
    g_GPIO_KEY_map_value[18] = 0x18;//#
    g_GPIO_KEY_map_value[19] = 0x19;//电话 拨号
    g_GPIO_KEY_map_value[20] = 0x27;//测试      
    g_GPIO_KEY_map_value[21] = 0x1A;//音量+     
    g_GPIO_KEY_map_value[22] = 0x1B;//音量-     
    g_GPIO_KEY_map_value[23] = 0x21;//PPT
    g_GPIO_KEY_map_value[24] = 0x20;//下
    g_GPIO_KEY_map_value[25] = 0x1F;//上
    g_GPIO_KEY_map_value[26] = 0x0C;//图示12
    g_GPIO_KEY_map_value[27] = 0x0B;//图示11
    g_GPIO_KEY_map_value[28] = 0x0A;//图示10
    g_GPIO_KEY_map_value[29] = 0x09;//图示9
    g_GPIO_KEY_map_value[30] = 0x08;//图示8
    g_GPIO_KEY_map_value[31] = 0x07;//图示7
    g_GPIO_KEY_map_value[32] = 0x1E;//外通
    g_GPIO_KEY_map_value[33] = 0x22;//左
    g_GPIO_KEY_map_value[34] = 0x23;//右
    g_GPIO_KEY_map_value[35] = 0x24;//ok
    g_GPIO_LED_map_value[0] = 0x26;//内通
    g_GPIO_LED_map_value[1] = 0x27;//图示1
    g_GPIO_LED_map_value[2] = 0x28;//图示2
    g_GPIO_LED_map_value[3] = 0x29;//图示3
    g_GPIO_LED_map_value[4] = 0x2A;//图示4
    g_GPIO_LED_map_value[5] = 0x2B;//图示5
    g_GPIO_LED_map_value[6] = 0x2C;//图示6
    g_GPIO_LED_map_value[7] = 0x2d;//1
    g_GPIO_LED_map_value[8] = 0x31;//4
    g_GPIO_LED_map_value[9] = 0x36;//7
    g_GPIO_LED_map_value[10] = 0x3C;//*
    g_GPIO_LED_map_value[11] = 0x3B;//0
    g_GPIO_LED_map_value[12] = 0x37;//8
    g_GPIO_LED_map_value[13] = 0x32;//5
    g_GPIO_LED_map_value[14] = 0x2e;//2
    g_GPIO_LED_map_value[15] = 0x2f;//3
    g_GPIO_LED_map_value[16] = 0x33;//6
    g_GPIO_LED_map_value[17] = 0x3F;//9
    g_GPIO_LED_map_value[18] = 0x3A;//#
    g_GPIO_LED_map_value[19] = 0x30;//电话 拨号
    g_GPIO_LED_map_value[20] = 0x34;//测试     
    g_GPIO_LED_map_value[21] = 0x3E;//音量+    
    g_GPIO_LED_map_value[22] = 0x39;//音量-    
    g_GPIO_LED_map_value[23] = 0x38;//PPT
    g_GPIO_LED_map_value[24] = 0x3D;//下
    g_GPIO_LED_map_value[25] = 0x35;//上
    g_GPIO_LED_map_value[26] = 0x40;//图示12
    g_GPIO_LED_map_value[27] = 0x41;//图示11
    g_GPIO_LED_map_value[28] = 0x42;//图示10
    g_GPIO_LED_map_value[29] = 0x43;//图示9
    g_GPIO_LED_map_value[30] = 0x44;//图示8
    g_GPIO_LED_map_value[31] = 0x45;//图示7
    g_GPIO_LED_map_value[32] = 0x46;//外通
    g_GPIO_LED_map_value[33] = 0x47;//左
    g_GPIO_LED_map_value[34] = 0x49;//右
    g_GPIO_LED_map_value[35] = 0x48;//ok
//    for (i = 0; i < 10; i++)
//    {
//        g_press_down_multi_num[i] = 0xff;
//    }
}





/*按键与按键灯值初始化*/
void set_board_map_value()
{
    if (g_card_device_type == 0 ||  g_card_device_type == 1 || g_card_device_type == 3  || g_card_device_type == 4)
    {
        GPIO_report_num_map_init_A();//A:对应的按键板型号为：嵌1、嵌2、壁挂2、防风雨
        g_LED_set_max_num = 0x46;
    }
    else if (g_card_device_type == 2) //小长方形
    {
        GPIO_report_num_map_init_B();//B:对应的按键板型号为：嵌3
        g_LED_set_max_num = 0x49;//待确认
    }
    else if (g_card_device_type == 5) //大长方形
    {
        GPIO_report_num_map_init_C();//C:对应的按键板型号为：多功能
        g_LED_set_max_num = 0x49;
    }
    else
    {
        GPIO_report_num_map_init_A();//默认为嵌1
        g_LED_set_max_num = 0x46;
        printf("can not scan board map,set default\n");
    }
	
//	for(i=0;i<33;i++)
//	{
//		printf("g_GPIO_KEY_map_value[%d] = %#x\r\n",i,g_GPIO_KEY_map_value[i]);
//	}
	
}




/*i2c0数据接收函数*/
uint8_t i2c0_Receive(uint8_t *rdata)
{
    uint8_t reallen;
    uint8_t i;
//    uint8_t temp[10];
    reallen = CIRC_RM_CNT(g_i2c0_rxbuf);
    if (reallen < 5)
    {
        return 0;
    }
    for (i = 0; i < reallen; i++)
    {
        CIRC_GET_CH(g_i2c0_rxbuf, rdata[0]);
        if (rdata[0] == 0x55)
		{
read_again:			
            CIRC_GET_CH(g_i2c0_rxbuf, rdata[1]);
			i++;
			if(rdata[1] == 0xaa)  //帧头验证成功
			{
				break;
			}
			else if(rdata[1] == 0x55)
			{
				goto read_again;
			}
		}
    }
	
	while( CIRC_RM_CNT(g_i2c0_rxbuf) < 3) //还需要3个数据
		vTaskDelay(3); //等待一下数据
	
	
    for (i = 2; i < 5; i++)
    {
        CIRC_GET_CH(g_i2c0_rxbuf, rdata[i]);
    }
//    memcpy(rdata, temp, 5);
//    memset(temp, 0, sizeof(temp));
    if ((CheckSum(rdata, 5) != rdata[4]))
    {
        printf("i2c recv data error\r\n");
		printf("data[0] = %#x,[1]= %#x,[2]= %#x,[3]= %#x,[4]= %#x\r\n",rdata[0],rdata[1],rdata[2],rdata[3],rdata[4]);
		//    CIRC_RELEASE(g_i2c0_rxbuf);
        return 0;
    }
    return (reallen);
}

/*寄存器复位函数*/
static void reset_handle(void)
{
//    memset(g_IIC_tx_data, 0, sizeof(g_IIC_tx_data));
//    g_IIC_tx_data[0] = 0x55;
//    g_IIC_tx_data[1] = 0xaa;
    AT9236_transmit_byte(0x4f, 0x00);  // reset
    AT9236_transmit_byte(0x00, 0x01);  // set shutdown register(0x00: software shutdown mode; 0x01: normal operation)
    AT9236_transmit_byte(0x4A, 0x00);  // set all channels enable
    AT9236_transmit_byte(0x4B, 0x01);  // Output Frequency Setting 22KHz(0x00:3KHz; 0x01:22KHz)
//    g_light_value = 4;
//    g_pwm_value = g_light_transform[g_light_value];
//    AT9236_PWM_control(g_pwm_value);
//    AT9236_LED_lightAll();
//    AT9236_transmit_byte(0x25, 0x00);  // update register
	
	set_keyleds_pwm(4);
    CIRC_RELEASE(g_i2c0_rxbuf);  //清空IIC1接收buf
}











/*寻找按键值*/
//static int find_index(uint8_t ch453_keyValue)
//{
//    int i;
//    for (i = 0; i < NUM_OF_KEY; i++)
//    {
//        if (g_CH453_AT9236_map[i].CH453_keyValue == ch453_keyValue)
//        {
//            return i;
//        }
//    }
//    return -1;
//}

/*查找上报按键值*/
//static uint8_t find_REPORT_keyValue(uint8_t ch453_keyValue)
//{
//    int i;
//    for (i = 0; i < NUM_OF_KEY; i++)
//    {
//        if (g_CH453_AT9236_map[i].CH453_keyValue == ch453_keyValue)
//        {
//            return g_CH453_AT9236_map[i].REPORT_keyValue;
//        }
//    }
//    return 0;
//}




#if 0
/*填充要发送的数据*/
void fill_tx_data(uint8_t txv2, uint8_t txv3, uint8_t txv4, uint8_t txv5)
{
    g_IIC_tx_buf[2] = txv2;
    g_IIC_tx_buf[3] = txv3;
    g_IIC_tx_buf[4] = txv4;
    g_IIC_tx_buf[5] = txv5;
    g_IIC_tx_buf[6] = CheckSum(g_IIC_tx_buf, 7);
	
//	printf("fill_tx_data:%#x,%#x,%#x,%#x,%#x,%#x,%#x\r\n",g_IIC_tx_buf[0],g_IIC_tx_buf[1],g_IIC_tx_buf[2]
//								,g_IIC_tx_buf[3],g_IIC_tx_buf[4],g_IIC_tx_buf[5],g_IIC_tx_buf[6]);
//	
    gpio_bit_reset(GPIOA, GPIO_PIN_2); //high->low
    gpio_bit_set(GPIOA, GPIO_PIN_2); //low->high
}
#else
/*填充要发送的数据*/


void fill_tx_data(uint8_t txv2, uint8_t txv3, uint8_t txv4, uint8_t txv5)
{
//	if(write_iic0_txbuf_bin) xSemaphoreTake(write_iic0_txbuf_bin, portMAX_DELAY);  //获取信号量
//	else printf("write_iic0_txbuf_bin is NULL \r\n");
	CIRC_PUT_CH(g_i2c0_txbuf, txv2);
	CIRC_PUT_CH(g_i2c0_txbuf, txv3);
	CIRC_PUT_CH(g_i2c0_txbuf, txv4);
	CIRC_PUT_CH(g_i2c0_txbuf, txv5);
//	xSemaphoreGive( write_iic0_txbuf_bin);    //释放信号量
	xTaskNotifyGive(TaskHandle_IIC0_SendData);  //唤醒发送数据任务
	//vTaskNotifyGiveFromISR(TaskHandle_IIC0_SendData,NULL);  //唤醒发送数据任务
	
//	printf("[6] = %#x\r\n",SwSlaveI2C.TxBuf[6]);   //必须要打印才行？2023-03-09
//	printf("txv2 = %#x,txv3 = %#x,txv4 = %#x,txv5 = %#x\r\n",
//		txv2,txv3,txv4,txv5);
	
}

#endif







//按键引脚初始化
void keys_scan_init(void)
{
//    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
	
    gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
              | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
              | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_init(GPIOD, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
              | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
              | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_init(GPIOE, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
	
}

static uint8_t s_old_key_val[36]= {0};


/*扫描按键*/
uint8_t scan_key_board()
{
    uint8_t i ;//, j , m  , k;
    uint8_t new_key_val[36] = {0};
    static uint8_t s_press_down_num = 0;

	
    for (i = 0; i < g_key_scan_set_num; i++)
    {
        if (RESET == gd_eval_key_state_get(i))   //低电平表示按下
        {
            new_key_val[i] = 1;//检测到按下，记录当前按键
        }
        else   //高电平表示松开
        {
            new_key_val[i] = 0;//没有检测到按下，恢复当前按键为0
        }
		
        if (new_key_val[i] != s_old_key_val[i])
        {
            if (new_key_val[i] > s_old_key_val[i])  //检测到按下
            {
                s_press_down_num++;
                g_save_presskey1 = g_GPIO_KEY_map_value[i];
                fill_tx_data(0x01, g_GPIO_KEY_map_value[i], 0x00, 0x00);
                printf("button %d down\r\n", i + 1);
//                for (j = 0; j < 3; j++)
//                {
//                    if (g_press_down_multi_num[j] == 0xff)
//                    {
//                        g_press_down_multi_num[j] = i;
//                        break;
//                    }
//                }
//                if (s_press_down_num > 1 && s_press_down_num <= 3)
//                {
//                    if (s_press_down_num == 2)
//                    {
//                        for (j = 0; j < 3; j++)
//                        {
//                            if (g_press_down_multi_num[j] != 0xff && g_press_down_multi_num[j] != i)
//                            {
//                                m = g_press_down_multi_num[j];
//                                break;
//                            }
//                        }
//                        printf("two button perss\r\n");
//                        fill_tx_data(0x01, g_GPIO_KEY_map_value[m], g_GPIO_KEY_map_value[i], 0x00);
//                    }
//                    else //s_press_down_num == 3
//                    {
//                        for (j = 0; j < 3; j++)
//                        {
//                            if (g_press_down_multi_num[j] != 0xff && g_press_down_multi_num[j] != i)
//                            {
//                                m = g_press_down_multi_num[j];
//                                break;
//                            }
//                        }
//                        for (j = 0; j < 3; j++)
//                        {
//                            if (g_press_down_multi_num[j] != 0xff && g_press_down_multi_num[j] != i && g_press_down_multi_num[j] != m)
//                            {
//                                k = g_press_down_multi_num[j];
//                                break;
//                            }
//                        }
//                        printf("three button perss\r\n");
//                        fill_tx_data(0x01, g_GPIO_KEY_map_value[m], g_GPIO_KEY_map_value[k], g_GPIO_KEY_map_value[i]);
//                    }
//                }
            }
            else  //松开
            {
			//	printf("new = %d old[%d] = %d\r\n",new_key_val[i],i, s_old_key_val[i]);
//                s_press_down_num--;
//                for (j = 0; j < 3; j++)
//                {
//                    if (g_press_down_multi_num[j] == i)
//                    {
//                        g_press_down_multi_num[j] = 0xff;
//                        break;
//                    }
//                }
                g_save_releasekey1 = g_GPIO_KEY_map_value[i];
                fill_tx_data(0x02, g_GPIO_KEY_map_value[i], 0x00, 0x00);
                printf("button %d up\r\n", i + 1);
            }
        }
        s_old_key_val[i] = new_key_val[i];
    }
    return 0;
}


/*任务2 扫描按键*/
void task2_func(void *pdata)
{
	uint8_t i = 0;
    printf("task2 scan_key_board start\r\n");
	//vTaskDelay(500);
	
	for(i=0;i<36;i++)
		s_old_key_val[i] = 0;
		
    while (1)
    {
        scan_key_board();
		vTaskDelay(50);
        //OSTimeDlyHMSM(0, 0, 0, 5);
    }
}


//ota-uart.c
void goto_ota_update(void);



/*任务3 接收主机指令并响应*/
void task3_func(void *pdata)
{
    uint8_t rx_data[7] = {0};
    uint8_t cmd_type, cmd;
	uint8_t flash_time;
	uint8_t i,checksum = 0;
	uint8_t *pdown_md5 = (void*)(UPDATE_FLAG_START_ADDR + DOWN_MD5_OFFET);
	uint32_t task_notity_val;
    printf("task3 comunicate cpu iic start\r\n");
	
	/*iic初始化*/
	IIC0_init()	;

    while (1)
    {
		ulTaskNotifyTake(ULONG_MAX,  //退出时，清除对应的位，0表示都不清零
						portMAX_DELAY); //无限等待
        if (i2c0_Receive(rx_data) > 0)
        {
            cmd_type = rx_data[2];
            cmd = rx_data[3];
			
		//	printf("cmd_type = %d,cmd = %d\r\n",cmd_type,cmd);
			
            switch (cmd_type)
            {
            case CMD_READ_PRESSKEY:
                fill_tx_data(0x02, 0x00, 0x00, g_save_presskey1);
                break;
            case CMD_READ_RELEASEKEY:
                fill_tx_data(0x02, 0x00, 0x00, g_save_releasekey1);
                break;
            case CMD_READ_LIGHT:
                fill_tx_data(0x10, 0x00, 0x00, (uint8_t)get_keyleds_pwm());
                break;
            case CMD_WRITE_LIGHT:
				task_notity_val = CMD_WRITE_LIGHT | (cmd<<8); 				
 							
                if (cmd > 0x64)
					cmd = 0x64;
				
                fill_tx_data(0x20, 0x00, 0x00, 0x5a);
				xTaskNotify(TaskHandle_Light_Control,task_notity_val,eSetValueWithOverwrite);

                break;
            case CMD_DEVICE_TYPE:
                fill_tx_data(0x30, 0x00, 0x00, g_card_device_type);
                break;
            case CMD_SOFT_VERSION:
                fill_tx_data(0x40, 0x00, 0x00, SOFT_VERSION);
                break;
            case CMD_RESET:
                reset_handle();
                fill_tx_data(0x50, 0x00, 0x00, 0x5A);
                break;
            case CMD_LIGHT_LED:
			//	printf("cmd_type = %d,cmd = %d\r\n",cmd_type,cmd);
				task_notity_val = CMD_LIGHT_LED | (cmd<<8);
				xTaskNotify(TaskHandle_Light_Control,task_notity_val,eSetValueWithOverwrite);
			
             //   g_IIC_tx_data[5] = 0x5a;//cmd_led_control(cmd, LED_ON);
                fill_tx_data(0x60, 0x00, 0x00, 0x5a);

                break;
            case CMD_LIGHT_OUT:
				task_notity_val = CMD_LIGHT_OUT | (cmd<<8); 
				xTaskNotify(TaskHandle_Light_Control,task_notity_val,eSetValueWithOverwrite);
              //  g_IIC_tx_data[5] = 0x5a;//cmd_led_control(cmd, LED_OFF);
                fill_tx_data(0x70, 0x00, 0x00, 0x5a);
                break;
			case CMD_UPDATE_MCU:   //单片机升级
				if(cmd == 0)  //上报md5，共35个字节
				{
					checksum = (uint8_t)(0x55+0xaa);
					for(i=0;i<32;i++)
					{
						g_IIC_tx_buf[i+2] = pdown_md5[i];
						checksum += pdown_md5[i];				
					}
					g_IIC_tx_buf[i+2] = checksum;   //写入缓存
					g_iic0_send_size = 35;
					//发送，产生中断
//					gpio_bit_reset(GPIOA, GPIO_PIN_2); //high->low
//					gpio_bit_set(GPIOA, GPIO_PIN_2); //low->high
				}
				else if(cmd == 1) //准备升级，直接设置后重启
				{
					goto_ota_update();
				}	
				else if(cmd == 2) //2023-05-10，查询编译时间及版本
				{
					checksum = (uint8_t)(0x55+0xaa);
					for(i=0;i<20;i++)
					{
						g_IIC_tx_buf[i+2] = g_build_time_str[11+i];  //前面几个字符不需要发送
						checksum += g_build_time_str[11+i];				
					}
					g_IIC_tx_buf[i+2] = SOFT_VERSION;  //加上版本号
					checksum += SOFT_VERSION;
					g_IIC_tx_buf[i+3] = checksum;   //校验和写入缓存
					g_iic0_send_size = 24;    //总共20个字符加上头部2，版本号，校验和1
				}				
				break;
            default:
				//led闪烁控制
				if((cmd_type & 0xfc) == CMD_LIGHT_FLASH)  //处理0x80,0x81,0x82,0x83
				{
					printf("cmd_type = %#x,cmd = %d\r\n",cmd_type,cmd);
					flash_time = cmd_type &0x3;   //闪烁频率
					light_leds_add_flash(cmd, flash_time);
					
					fill_tx_data(cmd_type, 0x00, 0x00, 0x5a);
				}
                break;
            }
        }
    }
}


/*任务4 闪烁LED灯*/
void task4_func(void *pdata)
{
    int run_led = 0;
    printf("task4_func start\r\n");
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);

    while (1)
    {
        run_led = ~run_led;
        LED_control(run_led);
		vTaskDelay(500);
    //    OSTimeDlyHMSM(0, 0, 0, 1000);
    }
}





//键灯控制任务，
void key_light_control_task(void *pdata)
{
	uint32_t value;
	uint8_t cmd,data;
	
	/*iic初始化*/
	IIC1_init();
//	IicApp_Init(IIC2_INDEX);   //通道iic初始化
	
//	CH453_AT9236_map_init();
    AT9236_transmit_byte(0x4f, 0x00);  // reset
    AT9236_transmit_byte(0x00, 0x01);  // set shutdown register(0x00: software shutdown mode; 0x01: normal operation)
    AT9236_transmit_byte(0x4A, 0x00);  // set all channels enable
    AT9236_transmit_byte(0x4B, 0x01);  // Output Frequency Setting 22KHz(0x00:3KHz; 0x01:22KHz)
    
	set_keyleds_pwm(4);   //设置开机后的默认亮度是4%
    AT9236_LED_lightAll();  //点亮全部的灯

	printf("key_light_control_task\r\n");
	
	for(;;)
	{
		xTaskNotifyWait(
				0,
				0,
				&value,
				portMAX_DELAY);

		
		cmd = value & 0xff;
		data = (value >> 8) & 0xff;
		//printf("key_light_control_task cmd = %d data = %d \r\n",cmd,data);
		switch (cmd)
		{
			case CMD_WRITE_LIGHT:
			{
			//	vTaskDelay(5);
				set_keyleds_pwm(data);
			}
			break;
			case CMD_LIGHT_LED:
			//	vTaskDelay(6);
            //    cmd_led_control(data, LED_ON);
				key_light_leds_control2(data, LED_ON);	//	data  --> 1-43	
                break;
            case CMD_LIGHT_OUT:
			//	vTaskDelay(6);
				key_light_leds_control2(data, LED_OFF);	
			//	cmd_led_control(data, LED_OFF);
                break;
		}
	}
	
}




/*读取按键板类型*/
void read_card_type(void)
{
    int i;
    uint32_t card_type_pin[6] = {GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12};
    for (i = 0; i < 6; i++)
    {
        if (gpio_input_bit_get(GPIOE, card_type_pin[i]) == SET)
        {
            g_card_device_type = i;
         //   printf("card type is %d\n", i);
            break;
        }
    }
    if (g_card_device_type == 0 ||  g_card_device_type == 1 || g_card_device_type == 3  || g_card_device_type == 4)
    {
        g_key_scan_set_num = 33;
    }
    else if (g_card_device_type == 2) //小长方形
    {
        g_key_scan_set_num = 33;
    }
    else if (g_card_device_type == 5) //大长方形
    {
        g_key_scan_set_num = 36;
    }
    else
    {
        g_key_scan_set_num = 33;
        printf("can not scan board num\n");
    }	
}







/*系统初始化*/
void bsp_sys_init(void)
{
//    SystickConfig(); //初始化时钟配置
    	//0. 中断分组初始化
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);    //2022-09-09 优先级被我修改了，现在只有抢占优先级了！！
	//串口首先初始化，用于打印调试，115200
	gd32_uart_init();  
	//
    gd32_gpio_init();
	
	//iic的初始化，调整到各自任务中去！！！
    //IIC_init();
    CIRC_RELEASE(g_i2c0_rxbuf);
    CIRC_RELEASE(g_i2c0_txbuf);
    

	
	//按键引脚初始化
	keys_scan_init();
    //获得面板类型
	read_card_type();
    //初始化键灯映射值
	set_board_map_value();
	
	//键灯闪烁控制
	key_light_leds_flash_init();
	
    printf("sys init finish 2023-04-06\r\n");
}




//800ms 看门狗
static void iwdog_init(void)
{
	fwdgt_write_enable();
	fwdgt_config(0xfff,FWDGT_PSC_DIV64);    //设置分配系数,最长819ms
	
//	fwdgt_enable(); //使能看门狗
}

//喂狗
static  void iwdog_feed(void)
{
//	if(mcu_reboot)  //设置mcu重启，不喂狗。2021-12-17增加
//		return ;
	fwdgt_counter_reload();
}




//喂狗任务
void feed_wtg_task(void* data)
{
	iwdog_init();
	
	printf("feed_wtg_task start\r\n");
	
	for(;;)
	{
		iwdog_feed();
		vTaskDelay(500);
	}
}










/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x5c00);   //注意变化！！！2023-05-06
	
//	StaticSemaphore_t  pxMutexBuffer;  
    bsp_sys_init();
	
	printf("freertos , init ok!!\r\n");	
	printf("%s\r\n",g_build_time_str);
	printf("Author:JC&DaZhi <vx:285408136>\r\n"); 
	
	//write_iic0_txbuf_mutex = xSemaphoreCreateMutex() ;  //创建互斥量
//	iic0_has_senddatas = xSemaphoreCreateBinary();
	
	//xSemaphoreGive( write_iic0_txbuf_mutex);    //释放信号量
	
	//3.led指示灯的任务
	xTaskCreate(task4_func,"TaskLed1",configMINIMAL_STACK_SIZE,NULL,1,NULL);
	//4.cpu通信IIC0任务 优先级要高一点，不然容易引起cpu端超时错误
	xTaskCreate(task3_func,"ToCpu",configMINIMAL_STACK_SIZE*3,NULL,4,&TaskHandle_ToCpu_IIC);  //cpu通信串口任务，优先级高一点  &TaskHandle_ToCpu_Com
	//5. 矩阵键盘扫描任务
	xTaskCreate(task2_func,"key_bod",configMINIMAL_STACK_SIZE*2,NULL,3,NULL);//&TaskHandle_key_Matrix);  //矩阵键盘扫描任务
	
	//6.控制键灯的任务
	xTaskCreate(key_light_control_task,"lights",configMINIMAL_STACK_SIZE*2,NULL,2,&TaskHandle_Light_Control);
	
	//7.发送iic0缓存的数据
	xTaskCreate(iic0_send_data_task,"iic0_send",configMINIMAL_STACK_SIZE*2,NULL,4,&TaskHandle_IIC0_SendData);
	
	
	//8.单片机内部看门狗
	xTaskCreate(feed_wtg_task,"wtg",configMINIMAL_STACK_SIZE,NULL,2,NULL);
	
	
	vTaskStartScheduler();
    
	
	
    while(1){}
	
}



/*计算校验和*/
uint8_t CheckSum(const uint8_t *buf, uint8_t len)
{
    uint8_t sum = 0;
    uint8_t i;
    for (i = 0; i < len - 1; i++)
        sum += buf[i];
    return sum;
}
