/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：AT9236.c
* 摘要：AT9236驱动
* 当前版本：1.4，zhaodazhi，2023-04-06
* 历史版本：无
*/

#include "AT9236.h"
#include "gpio.h"



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
void AT9236_PWM_control(uint8_t pwm)
{
    int i = 0;
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, 0x01);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    for (i = 0; i < 36; i++)
    {
        i2c_data_transmit(I2C_AT9236, pwm);
        while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
	AT9236_transmit_byte(0x25, 0x00);  // update register
}

/*点亮全部LED灯*/
void AT9236_LED_lightAll(void)
{
    int i = 0;
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, 0x26);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    for (i = 0; i < 36; i++)   //多功能型号是36个按键，其他型号是33个按键。
    {
        i2c_data_transmit(I2C_AT9236, 0x03);  //2023-03-15
        while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
	AT9236_transmit_byte(0x25, 0x00);  // update register
}

/*关闭全部LED灯*/
void AT9236_LED_lightAll_OFF(void)
{
    int i = 0;
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, 0x26);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    for (i = 0; i < 36; i++)   //多功能型号是36个按键，其他型号是33个按键。
    {
        i2c_data_transmit(I2C_AT9236, 0x00);
        while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
	
	AT9236_transmit_byte(0x25, 0x00);  // update register
}

/*LED灯单独控制*/
void AT9236_LED_control(uint8_t write_address, LED_control_t cmd)
{
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, write_address);  
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    
	if(cmd)
		cmd = 0x3;   //降低电流，2023-04-27
	i2c_data_transmit(I2C_AT9236, cmd);
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
	AT9236_transmit_byte(0x25, 0x00);  // update register
}

/*IIC发送数据至AT9236*/
void AT9236_transmit_byte(uint8_t write_address, uint8_t cmd)
{
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, write_address);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, (uint8_t)cmd);
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
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



/*寻找寄存器的值*/
static uint8_t find_AT9236_reg(uint8_t rpt_keyValue)
{
    int i;
    for (i = 0; i < g_key_scan_set_num; i++)
    {
        if (g_GPIO_KEY_map_value[i] == rpt_keyValue)
        {
		//	printf("i = %d g_GPIO_KEY_map_value[i] = %#x g_GPIO_LED_map_value[i] = %#x\r\n",i,rpt_keyValue,g_GPIO_LED_map_value[i]);
            return g_GPIO_LED_map_value[i];
        }
    }
//	printf("find_AT9236_reg none \r\n");
    return 0;
}




/*led控制指令解析函数
	cmd  1-43
*/
uint8_t cmd_led_control(uint8_t cmd, LED_control_t led_stat)
{
    uint8_t txv5 = 0x5A;
    if ((cmd > 0x00) && (cmd < 0x28))   //0-39
    {
        uint8_t at9236_reg_a = find_AT9236_reg(cmd);
		//printf("at9236_reg_a = %#x +++\r\n",at9236_reg_a);
        if (at9236_reg_a >= 0x26 && at9236_reg_a <= g_LED_set_max_num)
        {
		//	printf("at9236_reg_a xxxxx\r\n");
            AT9236_LED_control(at9236_reg_a, led_stat);
         //   AT9236_transmit_byte(0x25, 0x00);  // update register
        }
        else
        {
            txv5 = 0xA5;
            return txv5;
        }
    }
    else if ((cmd >= 0x28) && (cmd <= 0x2A))
    {
        if (led_stat == LED_OFF)
        {
			RGB_control_off((RGB_color_t)cmd);
            //RGB_control(RGB_ALL_OFF);
        }
        else
            RGB_control((RGB_color_t)cmd);
    }
    else if (cmd == 0x2B) /*lsr add 20220505*/
    {
        if (led_stat == LED_OFF)
        {
            AT9236_LED_lightAll_OFF();
			//	RGB_control(RGB_ALL_OFF);   //RGB_全部熄灭，2023-02-33,2023-03-02 注释，客户说不需要了
        }
        else
        {
            AT9236_LED_lightAll();
		//	RGB_control(RGB_ALL);   //RGB_全部点亮，2023-02-33,2023-03-02 注释，客户说不需要了
        }
        //AT9236_transmit_byte(0x25, 0x00);  // update register
    }
    else
    {
        txv5 = 0xA5;
        return txv5;
    }
    return txv5;
}
