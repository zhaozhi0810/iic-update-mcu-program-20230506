/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：uart.c
* 摘要：uart配置
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#include <stdio.h>
#include <string.h>
#include "uart.h"
//#include "gd32f10x_it.h"

/*!
    \brief      串口初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gd32_uart_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);  //uart0
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
	
	
		//5. 接收中断的初始化。
	usart_interrupt_enable(USART0, USART_INT_RBNE);    //接收中断
	//5.1 调试串口不再开启空闲中断。
//	if(com_id == TOCPU_COM_NUM)  //给cpu的通信使用缓存
//	{
//		usart_interrupt_enable(USART0, USART_INT_IDLE);    //空闲中断
//		Com_Cpu_Recive_Buff_Init();   //接收缓存初始化
//	}0
	//6. nvic的配置
	nvic_irq_enable(USART0_IRQn,  7, 0);   //允许中断，并设置优先级
		
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

