#ifndef __SW_SLAVE_I2C_H_
#define __SW_SLAVE_I2C_H_
 
#ifdef __cplusplus
extern "C" {
#endif
 
#include <gd32f10x.h>
 
#define SW_SLAVE_ADDR		0x80
 
// #define SW_SLAVE_SCL_CLK_EN()	__HAL_RCC_GPIOB_CLK_ENABLE()
// #define SW_SLAVE_SDA_CLK_EN()	__HAL_RCC_GPIOB_CLK_ENABLE()
 
#define SW_SLAVE_SCL_PRT		GPIOB
#define SW_SLAVE_SCL_PIN		GPIO_PIN_8
#define SW_SLAVE_SDA_PRT		GPIOB
#define SW_SLAVE_SDA_PIN		GPIO_PIN_9
 
#define GPIO_MODE_MSK            0x00000003U
 
#define I2C_STA_IDLE			0
#define I2C_STA_START			1
#define I2C_STA_DATA			2
#define I2C_STA_ACK			3
#define I2C_STA_NACK			4
#define I2C_STA_STOP			5
 
#define I2C_READ				1
#define I2C_WRITE			0
 
#define GPIO_DIR_IN			0
#define GPIO_DIR_OUT			1
 
#define SET_SCL_DIR(Temp, InOut)				\
	Temp = SW_SLAVE_SCL_PRT->MODER;			\
	Temp &= ~(GPIO_MODER_MODER6);			\
	Temp |= ((InOut & GPIO_MODE_MSK) << (6 * 2U));	\
	SW_SLAVE_SCL_PRT->MODER = temp;
 
#define SET_SDA_DIR(Temp, InOut)				\
	Temp = SW_SLAVE_SDA_PRT->MODER;			\
	Temp &= ~(GPIO_MODER_MODER7);			\
	Temp |= ((InOut & GPIO_MODE_MSK) << (7 * 2U));	\
	SW_SLAVE_SDA_PRT->MODER = Temp;
 
#define CLR_SDA_PIN()			(gpio_bit_reset(GPIOB, GPIO_PIN_9))
#define SET_SDA_PIN()			(gpio_bit_set(GPIOB, GPIO_PIN_9))
 
typedef struct _SwSlaveI2C_t
{
    uint8_t State;					// I2C通信状态
	uint8_t Rw;						// I2C读写标志：0-写，1-读
	uint8_t SclFallCnt;				// SCL下降沿计数
	uint8_t Flag;					// I2C状态标志，BIT0：0-地址无效，1-地址匹配
	uint32_t StartMs;				// I2C通信起始时间，单位ms，用于判断通信是否超时
//	uint8_t* RxBuf;					// 指向接收缓冲区的指针
	uint8_t TxBuf[16];					// 指向发送缓冲区的指针
	uint8_t RxIdx;					// 接收缓冲区数据写入索引，最大值255
	uint8_t TxIdx;					// 发送缓冲区数据读取索引，最大值255
}SwSlaveI2C_t;
 
extern volatile  SwSlaveI2C_t SwSlaveI2C;
 
void InitSwSlaveI2C0(void);
void I2cGpioIsr(void);
void CheckSwSlaveI2cTimeout(void);



//iic发送数据
void sned_soft_iic_rx_buf(uint8_t * buf,uint8_t len);
//返回值0，表示没有数据，>0表示字节数
uint8_t get_soft_iic_rx_buf(uint8_t * buf,uint8_t len);

#ifdef __cplusplus
}
#endif
 
#endif /* __SW_TIMER_H_ */

