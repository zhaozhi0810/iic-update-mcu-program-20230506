/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：gpio.c
* 摘要：gpio初始化配置
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#include "gpio.h"
//#include "systick.h"

/*
 * AT9236 SDB shutdown the chip when pulled low
*/
void AT9236_SDB_init(void)
{
    /* enable the clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure GPIO port */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_bit_set(GPIOB, GPIO_PIN_12); //high
}

/*AT9236 SDB控制*/
//void AT9236_SDB_control(uint8_t ctrlCode)
//{
//    if (ctrlCode)
//    {
//        gpio_bit_set(GPIOB, GPIO_PIN_12);
//    }
//    else
//    {
//        gpio_bit_reset(GPIOB, GPIO_PIN_12);
//    }
//}

/*CH453中断初始化*/
//void CH453_KEY_INT_init(void)
//{
//    /* enable the TAMPER key gpio clock */
//    rcu_periph_clock_enable(RCU_GPIOB);
//    /* configure button pin as input */
//    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
//    /* enable the AF clock */
//    rcu_periph_clock_enable(RCU_AF);
//    nvic_irq_enable(EXTI10_15_IRQn, 0U, 2U);
//    /* connect key EXTI line to key GPIO pin */
//    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_13);
//    /* configure key EXTI line */
//    exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);//EXTI_TRIG_BOTH);
//    exti_interrupt_flag_clear(EXTI_13);
//}

/*LED初始化*/
void LED_init(void)
{
    /* enable the clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure GPIO port */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    gpio_bit_set(GPIOB, GPIO_PIN_4);//high
}

/*LED控制*/
void LED_control(uint8_t ctrlCode)
{
    if (ctrlCode)
    {
        gpio_bit_set(GPIOB, GPIO_PIN_4);
    }
    else
    {
        gpio_bit_reset(GPIOB, GPIO_PIN_4);
    }
}

/*
RGB GPIO 初始化
R:PC0
G:PC1
B:PC2
*/
void RGB_GPIO_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    /* configure led GPIO port */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
    /*使用gpio_pin_remap_config配置GPIO_SWJ_SWDPENABLE_REMAP开启：PB3/4/5 use for GPIO, PB13/14 use for SWD*/
 //   gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
  //  gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);  // set GPIO pin
}

/*RGB GPIO 控制*/
void RGB_control(RGB_color_t color)
{
    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);  // set GPIO pin
    switch (color)
    {
    case RGB_R:
        GPIO_BC(GPIOC) = (uint32_t)GPIO_PIN_0;
    //    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_1 | GPIO_PIN_2);
        break;
    case RGB_G:
        GPIO_BC(GPIOC) = (uint32_t)GPIO_PIN_1;
    //    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_2);
        break;
    case RGB_B:
        GPIO_BC(GPIOC) = (uint32_t)GPIO_PIN_2;
    //    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1);
        break;
	case RGB_ALL:  //全亮，2023-02-22
//		gpio_bit_write(GPIOC, GPIO_PIN_1, RESET);
//		gpio_bit_write(GPIOC, GPIO_PIN_2, RESET);
//		gpio_bit_write(GPIOC, GPIO_PIN_0, RESET);		
		GPIO_BC(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);	

        break;
    default:   //全灭
        GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);  // set GPIO pin
    }
}


void RGB_control_off(RGB_color_t color)
{
//    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);  // set GPIO pin
    switch (color)
    {
    case RGB_R:
        GPIO_BOP(GPIOC) = (uint32_t)GPIO_PIN_0;
    //    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_1 | GPIO_PIN_2);
        break;
    case RGB_G:
        GPIO_BOP(GPIOC) = (uint32_t)GPIO_PIN_1;
    //    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_2);
        break;
    case RGB_B:
        GPIO_BOP(GPIOC) = (uint32_t)GPIO_PIN_2;
    //    GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1);
        break;
	default://全灭
	case RGB_ALL:  //全亮，2023-02-22
		GPIO_BOP(GPIOC) = (uint32_t)(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
        break;
    }
}



/*配置IIC0中断引脚*/
void IIC0_INT_gpio(void)
{
    /* enable the clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure GPIO port */
	gpio_bit_set(GPIOA, GPIO_PIN_2); //high
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_2);    
}

/*系统IO初始化*/
void gd32_gpio_init(void)
{
    AT9236_SDB_init();
    RGB_GPIO_init();
    LED_init();
    IIC0_INT_gpio(); //IIC0 INT
}

