/**
  ******************************************************************************
  * @file    IAP/src/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "common.h"

//iic0是否空闲
volatile uint8_t g_iic0_is_Idle = 1;  //0表示非空闲，1表示空闲


/** @addtogroup IAP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 



void I2C0_EV_IRQHandler(void)
{
    static uint8_t i = 0;
	uint8_t tdata,tdata_count;
//    static uint8_t j = 0;
//    static uint8_t k = 0;
//    static uint8_t reallen = 0;
    /*收到地址请求*/
    if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND))
    {
        /* clear the ADDSEND bit */
	//	g_iic0_is_Idle = 0;  //0表示非空闲，1表示空闲
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_ADDSEND);
    //    i = 0;
//        i2c_data_transmit(I2C0, g_IIC_tx_buf[i++]);
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_RBNE))
    {
	//	g_iic0_is_Idle = 0;  //0表示非空闲，1表示空闲
        /* if reception data register is not empty ,I2C0 will read a data from I2C_DATA */
        CIRC_PUT_CH(g_i2c0_rxbuf, i2c_data_receive(I2C0));
    }
    else if ((i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))&& !i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_AERR) )  // 
    {
        /*收到主机读取信号，并且没有错误，则发送数据*/
        /* send a data byte */
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_TBE);
//		if(i==0)
//		{
//			tdata_count = CIRC_RM_CNT(g_i2c0_txbuf);
//			i2c_data_transmit(I2C0, tdata_count);     //发送缓存字节数
//			i++;
//		}		
//		else 
		i++;
		tdata_count = CIRC_RM_CNT(g_i2c0_txbuf);
		if( tdata_count > 0 && (i%2))   //发送缓存有数据
		{
		//	g_iic0_is_Idle = 0;
			CIRC_GET_CH(g_i2c0_txbuf, tdata);   //取出数据，
			//printf("tdata = %#x,i=%d\r\n",tdata,i++);
			i2c_data_transmit(I2C0, tdata);     //发送出去
		}
		else
			i2c_data_transmit(I2C0, 0); 
			
//			if (i < 7) //继续发送
//			{
//				i2c_data_transmit(I2C0, g_IIC_tx_buf[i++]);
//				//i++;
//				if (i == 7)
//				{
//					g_iic0_is_Idle = 1;  //0表示非空闲，1表示空闲

//					i = 0;
//				}
//			}
		
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_STPDET))  //收到停止标志
    {
        /*收到主机发送的停标志位*/
        //i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_STPDET);
        /* clear the STPDET bit */
        i2c_enable(I2C0);
        /* disable I2C0 interrupt */
        i2c_interrupt_disable(I2C0, I2C_INT_ERR);
        i2c_interrupt_disable(I2C0, I2C_INT_BUF);
        i2c_interrupt_disable(I2C0, I2C_INT_EV);
		g_iic0_is_Idle = 1;  //0表示非空闲，1表示空闲
	//	printf("111\r\n");
	//	i = 0;
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_BTC))
    {
        /*收到字节传输结束标志位*/
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_BTC);
    }
    else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))  //发送后，主机没有应答
    {
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_TBE);
		g_iic0_is_Idle = 1;
	//	printf("222\r\n");
	//	i = 0;
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




/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
