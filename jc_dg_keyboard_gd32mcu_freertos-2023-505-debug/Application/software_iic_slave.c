/*
* @Author: dazhi
* @Date:   2023-03-02 14:06:18
* @Last Modified by:   dazhi
* @Last Modified time: 2023-03-02 15:26:32
*/

//GPIOB8，9
#include "includes.h"

volatile SwSlaveI2C_t SwSlaveI2C;


//uint8_t volatile tx_buf[32];
//uint8_t rx_buf[32];



static void Soft_Iic0_Slave_Sda_In(void)
{
//	gpio_bit_set(GPIOB, GPIO_PIN_9);   //输入时。OD模式应该输出高就可以了
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ,GPIO_PIN_9);
}


static void Soft_Iic0_Slave_Sda_Out(void)
{
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ,GPIO_PIN_9); 
}

//static void Scl_In(void)
//{
//    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ,GPIO_PIN_8);
//}


//static void Scl_Out(void)
//{
//    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ,GPIO_PIN_8); 
//}



void InitSwSlaveI2C0(void)
{
//	uint32_t pin;
	//1. 时钟使能
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_AF);	
	//2. 设置为输入模式	
//	pin = BITS(8,9); 
	
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, GPIO_PIN_8);   //scl
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, GPIO_PIN_9);   //sda
	
	//2.2 复用为外部中断引脚，
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_8);
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_9);

	//设置触发方式，低电平触发
	exti_init(EXTI_8, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
	exti_init(EXTI_9, EXTI_INTERRUPT, EXTI_TRIG_FALLING);   //sda  EXTI_TRIG_FALLING 
	exti_interrupt_flag_clear(EXTI_8);    //SCL
	exti_interrupt_flag_clear(EXTI_9);    //SDA
	exti_interrupt_enable(EXTI_8);
	exti_interrupt_enable(EXTI_9);
	//2.3 nvic允许中断
	//中断控制器使能，使用的是外部中断12
	nvic_irq_enable(EXTI5_9_IRQn,  5, 0);   //允许中断，并设置优先级
	
//	SwSlaveI2C.RxBuf =  rx_buf;
//	SwSlaveI2C.TxBuf =  tx_buf;
		
}





void EXTI5_9_IRQHandler(void)
{
  I2cGpioIsr();
}

void I2cGpioIsr(void)
{
	static uint8_t recv_buf = 0;
  //  uint32_t temp;
	// 处理SCL的上下沿中断
	if(exti_flag_get(EXTI_8) != RESET)
	{
		exti_flag_clear(EXTI_8);
		// 更新通信起始时间
		SwSlaveI2C.StartMs = SysTick->VAL;
		// SCL的下降沿事件处理，此时需要更新要传输的数据
		if(gpio_input_bit_get(GPIOB, SW_SLAVE_SCL_PIN) == RESET)   //SCL为低电平
		{
			//1. SCL 为低，表示由高变低，
			//   1.1 接收主机数据模式时不需要关注，
			//   1.2 发送数据时需要把数据放到SDA总线上
			//2. SCL 为高，表示由低变高，锁存数据
			//   2.1 接收主机数据模式时读取数据位，
			//   2.2 发送数据时不需要关注
			
			
			switch(SwSlaveI2C.State)  //准备传输
			{
				case I2C_STA_START:    // 起始信号的下降沿，初始化相关参数并转到接收比特数据状态
					SwSlaveI2C.SclFallCnt = 0;
					SwSlaveI2C.RxIdx = 0;
					SwSlaveI2C.TxIdx = 0;
					SwSlaveI2C.Flag = 0;  // 默认地址不匹配
					//SwSlaveI2C.RxBuf[SwSlaveI2C.RxIdx] = 0;
					recv_buf = 0;	
					SwSlaveI2C.Rw = I2C_WRITE;  // 第1字节为设备地址，一定是写入
					SwSlaveI2C.State = I2C_STA_DATA;
				break;
				case I2C_STA_DATA:
					SwSlaveI2C.SclFallCnt++;  //数据位++
					if(8 > SwSlaveI2C.SclFallCnt)//还没有收到8个bit
					{
						// 如果是主机读取数据，则在SCL低电平时更新比特数据
						if(SwSlaveI2C.Rw == I2C_READ)  //需要发送数据
						{
							if(SwSlaveI2C.TxBuf[SwSlaveI2C.TxIdx] & (1 << (7 - SwSlaveI2C.SclFallCnt)))
							{
								SET_SDA_PIN();
							}
							else
							{
								CLR_SDA_PIN();
							}
						}
						//接收数据不用关心！！！
					}
					else if(8 == SwSlaveI2C.SclFallCnt)  //收到8bits
					{
						if(SwSlaveI2C.Rw == I2C_WRITE)//对方写到单片机中来
						{
							// 从第一个地址字节中获取读写标志位，并判断地址是否匹配
							if(SwSlaveI2C.RxIdx == 0)
							{
								if((recv_buf & 0xFE) == SW_SLAVE_ADDR)
								{
									SwSlaveI2C.Flag = 1;  // 地址匹配
									SwSlaveI2C.Rw = recv_buf & 0x01;   //修改读写模式
//									Soft_Iic0_Slave_Sda_Out();
//									CLR_SDA_PIN();  //准备应答数据
								}
							}
							else if(SwSlaveI2C.Flag)//地址匹配 数据写入缓存
							{
								CIRC_PUT_CH(g_i2c0_rxbuf, recv_buf);
							}
							if(SwSlaveI2C.Flag)  // 地址匹配
							{
								// 如果是主机写入数据，且地址匹配，则接收完8比特数据后，需要发送ACK信号进行应答
								//SET_SDA_DIR(temp, GPIO_DIR_OUT);
								Soft_Iic0_Slave_Sda_Out();
								CLR_SDA_PIN();  //准备应答数据
							}
						}
						else //对方读取单片机的数据
						{
							// 如果是主机读取数据，需要将SDA设置成输入以便判断应答标志位状态
							Soft_Iic0_Slave_Sda_In();
							//SET_SDA_DIR(temp, GPIO_DIR_IN);
							// 如果是主机读取数据，准备发送下一个字节的数据
							SwSlaveI2C.TxIdx++;
						}
						// 接收或发送完8比特数据后，准备发送或接收应答信号
						SwSlaveI2C.State = I2C_STA_ACK;
					}
				break;
				case I2C_STA_ACK:  //应答阶段
				SwSlaveI2C.SclFallCnt = 0;
				if(SwSlaveI2C.Rw == I2C_WRITE)//对方写到单片机中来,输出应答
				{
					// 如果是主机写入数据，且ACK发送完毕，则SDA设置成输入，继续接收数据
					Soft_Iic0_Slave_Sda_In();
					//SET_SDA_DIR(temp, GPIO_DIR_IN);
					SwSlaveI2C.RxIdx++;
					recv_buf = 0;//SwSlaveI2C.RxBuf[SwSlaveI2C.RxIdx] = 0;					
				}
				else  //单片机输出数据,接收应答完成
				{
					// 如果是主机读取数据，且ACK接收完毕，则SDA设置成输出，继续发送数据
					Soft_Iic0_Slave_Sda_Out();
					//SET_SDA_DIR(temp, GPIO_DIR_OUT);
					if(SwSlaveI2C.TxBuf[SwSlaveI2C.TxIdx] & 0x80)
					{
						SET_SDA_PIN();
					}
					else
					{
						CLR_SDA_PIN();
					}
				}
				SwSlaveI2C.State = I2C_STA_DATA;
				break;
				case I2C_STA_NACK:    // 如果收到了NACK，则后面将是STOP或者ReSTART信号，需要将SDA设置成输入
					SwSlaveI2C.SclFallCnt = 0;
					SwSlaveI2C.Flag = 0;   //清除地址匹配
					//SET_SDA_DIR(temp, GPIO_DIR_IN);
					Soft_Iic0_Slave_Sda_In();
				break;
			}
		}
		// SCL的上升沿事件处理，此时需要采集数据，而且在数据阶段，SCL高电平时数据必须保持不变
		else  //SCL 为高
		{
			switch(SwSlaveI2C.State)
			{
				case I2C_STA_DATA:  // 数据阶段，如果是主机写入数据，则采集比特数据
				if((I2C_WRITE == SwSlaveI2C.Rw) && (8 > SwSlaveI2C.SclFallCnt))
				{
					if(gpio_input_bit_get(GPIOB, SW_SLAVE_SDA_PIN))
					{
						recv_buf |= (1 << (7 - SwSlaveI2C.SclFallCnt));//SwSlaveI2C.RxBuf[SwSlaveI2C.RxIdx] |= (1 << (7 - SwSlaveI2C.SclFallCnt));
					}
				}
				break;
				case I2C_STA_ACK:  // 应答阶段，如果是主机读取数据，则判断ACK/NACK信号，默认状态是ACK
				if((SwSlaveI2C.Rw == I2C_READ) && (gpio_input_bit_get(GPIOB, SW_SLAVE_SDA_PIN)))
				{
					SwSlaveI2C.State = I2C_STA_NACK;
				}
				break;
			}
		}
	}
	else if(exti_flag_get(EXTI_9) != RESET)  //SDA中断
	{
		//1.SCL为高时，SDA由高变低，起始信号
		//2.SCL为高时，SDA由低变高，结束信号
		//3.SCL为低时，SDA变化是主机发送数据，不用读取数据
		
		exti_flag_clear(EXTI_9);
		if(gpio_input_bit_get(GPIOB, SW_SLAVE_SCL_PIN))//SCL为高
		{
			if(gpio_input_bit_get(GPIOB, SW_SLAVE_SDA_PIN) == RESET)   //SDA 为低，表示是由高变低，起始信号
			{
				// SCL为高电平时，SDA从高变低，说明是起始信号				
				SwSlaveI2C.State = I2C_STA_START;
				exti_init(EXTI_9, EXTI_INTERRUPT, EXTI_TRIG_RISING);  //sda切换为上升沿
				//可以关闭SDA的中断了？停止信号还是需要
				
			}
			else// SCL为高电平时，SDA从低变高，说明是停止信号，一次I2C通信结束，直接将状态设置成空闲	
			{							
				SwSlaveI2C.State = I2C_STA_IDLE;
				SwSlaveI2C.Flag = 0;   //清除地址匹配
				exti_init(EXTI_9, EXTI_INTERRUPT, EXTI_TRIG_FALLING);  //sda切换为下降沿
				vTaskNotifyGiveFromISR(TaskHandle_ToCpu_IIC,NULL);  //唤醒任务				
			}
		}		
	}
}


//返回值0，表示没有数据，>0表示字节数
//uint8_t get_soft_iic_rx_buf(uint8_t * buf,uint8_t len)
//{
//	uint8_t i;
//	
//	for(i=0;(i<len && i<SwSlaveI2C.RxIdx);i++)
//	{
//		buf[i] = SwSlaveI2C.RxBuf[i];
//	}
//	
//	return i;
//}


//iic发送数据
//void sned_soft_iic_rx_buf(uint8_t * buf,uint8_t len)
//{
//	uint8_t i;
//	
//	for(i=0;(i<len);i++)
//	{
//		SwSlaveI2C.RxBuf[i] = buf[i];
//	}
//	
//	//触发中断引脚
//	gpio_bit_reset(GPIOA, GPIO_PIN_2); //high->low
//    gpio_bit_set(GPIOA, GPIO_PIN_2); //low->high
//	
//	return ;
//}


