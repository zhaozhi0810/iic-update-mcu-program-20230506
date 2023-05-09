/**
  ******************************************************************************
  * @file    IAP/src/download.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides the software which allows to download an image 
  *          to internal Flash.
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

/** @addtogroup IAP
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t file_name[FILE_NAME_LENGTH];
//uint8_t tab_1024[1024] ={0};
uint8_t md5sum_down[34] ={0};  //存放md5值
int32_t Size = 0;   //存放固件大小，下载的值
extern uint8_t is_cpu_update_cmd;   //是rk3399的升级吗？ 非0表示从rk3399下载

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval 0 表示成功，其他表示失败
  */
uint8_t SerialDownload(void)
{
	uint8_t Number[10] = "          ";
	
	char str[20];

	SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	Size = Ymodem_Receive();//&tab_1024[0]
	sprintf( str , "\r\nsize=%d\r\n" , Size );
	SerialPutString( str );
	if (Size > 0)
	{
		SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
		SerialPutString(file_name);
		Int2Str(Number, Size);
		SerialPutString("\n\r Size: ");
		SerialPutString(Number);
		SerialPutString(" Bytes\r\n");
		if(is_cpu_update_cmd)  //rk3399下载才有md5码
		{
			SerialPutString("md5sum : ");
			SerialPutString(md5sum_down);
			SerialPutString(" \r\n");
		}
		SerialPutString("-------------------\n");
		return 0;
	}
	else if (Size == -1)
	{
		SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
	}
	else if (Size == -2)
	{
		SerialPutString("\n\n\rVerification failed!\n\r");
	}
	else if (Size == -3)
	{
		SerialPutString("\r\n\nAborted by user.\n\r");
	}
	else
	{
		SerialPutString("\n\rFailed to receive the file!\n\r");
	}
	
	return 1;
}

/**
  * @}
  */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
