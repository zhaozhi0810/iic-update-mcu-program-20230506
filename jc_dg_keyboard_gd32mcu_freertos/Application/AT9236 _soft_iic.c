/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：AT9236.c
* 摘要：AT9236驱动
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#include "includes.h"
#include "AT9236.h"



#define I2C_AT9236         I2C1
#define I2C1_AT9236_ADDR   0x78


static uint8_t g_light_value = 0;/*按键亮度,0-100*/


/*亮度表*/
static uint8_t g_light_transform[101] =
{
    0,    15,   25,   35,   50,
    60,   62,   64,   66,   68,
    70,   72,   74,   76,   78,
    80,   82,   84,   86,   88,
    90,   92,   94,   96,   98,
    100,  102,  104,  106,  108,
    110,  112,  114,  116,  118,
    120,  122,  124,  126,  128,
    130,  132,  134,  136,  138,
    140,  142,  144,  146,  148,
    150,  152,  154,  156,  158,
    160,  162,  164,  166,  168,
    170,  172,  174,  176,  178,
    180,  182,  184,  186,  188,
    190,  192,  194,  196,  198,
    200,  202,  204,  206,  208,
    210,  212,  214,  216,  218,
    220,  222,  224,  226,  228,
    230,  232,  234,  236,  238,
    240,  242,  244,  246,  248, 250
};




/*设置PWM亮度*/
static void AT9236_PWM_control(uint8_t pwm)
{
    int i = 0;
    uint8_t buf[36] = {pwm};

    for (i = 0; i < 36; i++)
    {
        buf[i] = pwm;
		//IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x1+i,buf,1);
    }	
    IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x01,buf,36);
	buf[0] = 0;
	IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x25,buf,1);
}



//设置键灯的亮度pwm【0-100】
void set_keyleds_pwm(uint8_t pwm)
{
	uint8_t pwm_value;  
	
	if(pwm > 100)
		pwm = 100;
	
	g_light_value = pwm;  //保存到全局变量
    pwm_value = g_light_transform[g_light_value];
    AT9236_PWM_control(pwm_value);
}

//获取按键灯的亮度值
uint8_t get_keyleds_pwm(void)
{
	return g_light_value;
}

/*点亮全部LED灯*/
void AT9236_LED_lightAll(void)
{
    int i = 0;
    uint8_t buf[36] = {1};

    for (i = 0; i < 36; i++)
    {
        buf[i] = 3;
		//IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x26+i,buf,1);
    }

    IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x26,buf,36);
	buf[0] = 0;
	IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x25,buf,1);
}

/*关闭全部LED灯*/
void AT9236_LED_lightAll_OFF(void)
{
    int i = 0;
    uint8_t buf[36] = {0};

    for (i = 0; i < 36; i++)
    {
        buf[i] = 0;
		//IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x26+i,buf,1);
    }

    IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x26,buf,36);
//	buf[0] = 0;
	IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,0x25,buf,1);
	
}

/*IIC发送数据至AT9236*/
void AT9236_transmit_byte(uint8_t write_address, uint8_t cmd)
{
	IicApp_Write_Bytes(IIC2_INDEX,I2C1_AT9236_ADDR,write_address,&cmd,1);
    
}


/*LED灯单独控制*/
void AT9236_LED_control(uint8_t write_address, LED_control_t cmd)
{
    AT9236_transmit_byte( write_address,  cmd);
}


