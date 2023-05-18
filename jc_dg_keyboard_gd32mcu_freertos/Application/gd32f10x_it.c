/*!
    \file    gd32f10x_it.c
    \brief   interrupt service routines

    \version 2014-12-26, V1.0.0, firmware for GD32F10x
    \version 2017-06-20, V2.0.0, firmware for GD32F10x
    \version 2018-07-31, V2.1.0, firmware for GD32F10x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

//#include "gd32f10x_it.h"
//#include "main.h"
//#include "systick.h"
#include "includes.h"



extern IIC_CircleBuffer g_i2c0_rxbuf;
//extern uint8_t g_IIC_tx_buf[36];
extern uint8_t i2c0_send_mark;

//iic0是否空闲
volatile uint8_t g_iic0_is_Idle = 1;  //0表示非空闲，1表示空闲
volatile uint8_t g_iic0_has_senddatas = 0;  //0表示没有，1表示有数据
volatile uint8_t g_iic0_send_size = 7;  //默认是发送7个字节

uint8_t num_test;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/

//运行异常跳转口，使用cm_backtrace组件之后屏蔽掉
//cm_backtrace组件实现对异常处理的打印
//void HardFault_Handler(void)
//{
//    /* if Hard Fault exception occurs, go to infinite loop */
//    while (1)
//    {
//    }
//}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
//void SVC_Handler(void)
//{
//}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
//使用ucos的时候要注意屏蔽掉，这里是软中断,ucos已经实现
//void PendSV_Handler(void)
//{
//}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
//void SysTick_Handler(void)
//{
//    OSIntEnter();                                //进入中断
//    OSTimeTick();                           //调用ucos的时钟服务程序
//    delay_decrement();
//    OSIntExit();                            //触发任务切换软中断
//}


void I2C0_EV_IRQHandler(void)
{
    static uint8_t i = 0;
//    static uint8_t j = 0;
//    static uint8_t k = 0;
//    static uint8_t reallen = 0;
    /*收到地址请求*/
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND))
    {
        /* clear the ADDSEND bit */
		g_iic0_is_Idle = 0;  //0表示非空闲，1表示空闲
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_ADDSEND);
        i = 0;
//        i2c_data_transmit(I2C0, g_IIC_tx_buf[i++]);
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_RBNE))
    {
		g_iic0_is_Idle = 0;  //0表示非空闲，1表示空闲
        /* if reception data register is not empty ,I2C0 will read a data from I2C_DATA */
        CIRC_PUT_CH(g_i2c0_rxbuf, i2c_data_receive(I2C0));
    }
    else if ((i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE)) && !i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_AERR))  //主机有应答 
    {
        /*收到主机读取信号，并且没有错误，则发送数据*/
        /* send a data byte */		
		if (i < g_iic0_send_size) //继续发送
		{
			i2c_data_transmit(I2C0, g_IIC_tx_buf[i++]);   //TBE 写入数据后，自动清零
			//i++;
			if (i == g_iic0_send_size)
			{
				g_IIC_tx_buf[2] = 0;
				g_IIC_tx_buf[3] = 0;
				g_IIC_tx_buf[4] = 0;
				g_IIC_tx_buf[5] = 0;
				g_IIC_tx_buf[6] = 0;
				g_iic0_is_Idle = 1;  //0表示非空闲，1表示空闲
				vTaskNotifyGiveFromISR(TaskHandle_IIC0_SendData,NULL);  //唤醒发送数据任务
				i = 0;
				if(g_iic0_send_size > 7)
					g_iic0_send_size = 7;
			}
		}
		else
			i2c_data_transmit(I2C0, 0);
		
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_STPDET))
    {
        /*收到主机发送的停标志位*/
        //i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_STPDET);
        /* clear the STPDET bit */
        i2c_enable(I2C0);
        /* disable I2C0 interrupt */
        i2c_interrupt_disable(I2C0, I2C_INT_ERR);
        i2c_interrupt_disable(I2C0, I2C_INT_BUF);
        i2c_interrupt_disable(I2C0, I2C_INT_EV);
		vTaskNotifyGiveFromISR(TaskHandle_ToCpu_IIC,NULL);  //唤醒任务
		g_iic0_is_Idle = 1;  //0表示非空闲，1表示空闲
		vTaskNotifyGiveFromISR(TaskHandle_IIC0_SendData,NULL);  //唤醒发送数据任务
		i = 0;
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_BTC))
    {
        /*收到字节传输结束标志位*/
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_BTC);
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))  //主机没有应答
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_TBE);
		i= 0;
    }
    i2c_interrupt_enable(I2C0, I2C_INT_ERR);
    i2c_interrupt_enable(I2C0, I2C_INT_EV);
    i2c_interrupt_enable(I2C0, I2C_INT_BUF);
}

void I2C0_ER_IRQHandler(void)
{
    //      printf("I2C0_ER_IRQHandler\n");
    /* no acknowledge received */
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_AERR))
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_AERR);
    }
    /* SMBus alert */
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_SMBALT))
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_SMBALT);
    }
    /* bus timeout in SMBus mode */
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_SMBTO))
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_SMBTO);
    }
    /* over-run or under-run when SCL stretch is disabled */
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_OUERR))
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_OUERR);
    }
    /* arbitration lost */
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_LOSTARB))
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_LOSTARB);
    }
    /* bus error */
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_BERR))
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_BERR);
    }
    /* CRC value doesn't match */
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_PECERR))
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_PECERR);
    }
}




//串口0 用于与程序员进行调试通信使用
void USART0_IRQHandler(void)
{		
	if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
	{
		Com_Debug_Rne_Int_Handle();
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);   //清中断标志
	}
	else if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE))  //空闲中断，表示一帧数据已结束
	{
		//解析命令，并处理。
	//	Com_Debug_Idle_Int_Handle();
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);//清中断标志		
	}
}


