/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：iic.c
* 摘要：自定义iic配置
* 当前版本：1.1，赵智、20230506
* 历史版本：1.0，蒋润东、20220628
*/
#include "common.h"
//#include "soft_iic_master_app.h"
//#include "software_iic_slave.h"


#define I2C0_SLAVE_ADDRESS7     0x80

#define I2C0_SPEED              100000


IIC_CircleBuffer g_i2c0_rxbuf;/*I2C接收缓冲区*/
IIC_CircleBuffer g_i2c0_txbuf;/*I2C发送缓冲区*/
uint8_t g_IIC_tx_buf[7] = {0x55, 0xaa};   /*I2C发送缓存*/



/*计算校验和*/
uint8_t CheckSum(const uint8_t *buf, uint8_t len)
{
    uint8_t sum = 0;
    uint8_t i;
    for (i = 0; i < len - 1; i++)
        sum += buf[i];
    return sum;
}




/*!
    \brief      cofigure the GPIO ports.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC0_gpio_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_I2C0_REMAP, ENABLE);
    /* I2C0 GPIO ports */
    /* connect PB8 to I2C0_SCL */
    /* connect PB9 to I2C0_SDA */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);
}

/*!
    \brief      cofigure the I2C0 and I2C1 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC0_config(void)
{
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C0);
    /* I2C clock configure */
    i2c_clock_config(I2C0, I2C0_SPEED, I2C_DTCY_2);
    /* I2C address configure */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
    /* enable I2C0 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}




/*!
    \brief      cofigure the NVIC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC0_IRQ_config(void)
{
    nvic_irq_enable(I2C0_EV_IRQn, 6, 1);
    nvic_irq_enable(I2C0_ER_IRQn, 6, 1);
    /* enable the I2C0 interrupt */
    i2c_interrupt_enable(I2C0, I2C_INT_ERR);
    i2c_interrupt_enable(I2C0, I2C_INT_EV);
    i2c_interrupt_enable(I2C0, I2C_INT_BUF);
}





/*iic初始化*/
void IIC0_init(void)
{	
    IIC0_gpio_config();
    IIC0_config();
    IIC0_IRQ_config();   //使用中断
//	InitSwSlaveI2C0();
	
	CIRC_RELEASE(g_i2c0_rxbuf);  //清空IIC0接收buf
	CIRC_RELEASE(g_i2c0_txbuf);  //清空IIC0发送buf
	
}






#if 1
/*i2c0数据接收函数*/
//uint8_t i2c0_get_txdata(uint8_t *rdata)
//{
//    uint8_t reallen;
//    uint8_t i;
////    uint8_t temp[10];
////    reallen = CIRC_RM_CNT(g_i2c0_txbuf);

//    if (reallen < 4)   //发送有7个字节，去掉帧头2个字节0x55，0xaa，和末尾的校验和，只剩下4个字节的数据。
//    {
//        return 0;
//    }
//	
//    for (i = 0; i < 4; i++)//只读4个字节
//    {
// //       CIRC_GET_CH(g_i2c0_txbuf, rdata[i]);
//    }

//    return (4);
//}




/*填充要发送的数据*/
//void iic0_send_data(uint8_t *buf,uint8_t len)
//{
////	g_IIC_tx_buf[0] = 0x55;
////	g_IIC_tx_buf[1] = 0xaa;
//	g_IIC_tx_buf[2] = buf[0];
//    g_IIC_tx_buf[3] = buf[1];
//    g_IIC_tx_buf[4] = buf[2];
//    g_IIC_tx_buf[5] = buf[3];
//    g_IIC_tx_buf[6] = CheckSum((void*)g_IIC_tx_buf, 7);
//	
//	gpio_bit_reset(GPIOA, GPIO_PIN_2); //high->low
//    gpio_bit_set(GPIOA, GPIO_PIN_2); //low->high
//	
//}


/*任务5  把iic缓存的东西发出去
*/


//正常返回0，否则返回-1
int32_t i2c0_Receive(uint8_t *rdata,uint16_t len,uint32_t timeout)
{
    uint16_t reallen;
    uint16_t i;
//    uint8_t temp[10];
	
	while (--timeout > 0)
	{
		reallen = CIRC_RM_CNT(g_i2c0_rxbuf);
		if (reallen >= len)
		{
			break;
		}
	}
	if (reallen < len)
		return -1;

    for (i = 0; i < len; i++)
    {
        CIRC_GET_CH(g_i2c0_rxbuf, rdata[i]);
	}
	
	return 0;
}



//获取一个字符
uint32_t i2c0_Receive_byte(uint8_t *key)
{
	if(CIRC_RM_CNT(g_i2c0_rxbuf)<=0) 
		return 0;
	CIRC_GET_CH(g_i2c0_rxbuf, *key);
	return 1;
}

//iic只发送一个字节，
uint32_t Send_Byte_to_iic0 (uint8_t c)
{
	CIRC_PUT_CH(g_i2c0_txbuf, c);   //数据写入缓存
//	g_IIC_tx_buf[0] = c;	
//	gpio_bit_reset(GPIOA, GPIO_PIN_2); //high->low
//    gpio_bit_set(GPIOA, GPIO_PIN_2); //low->high
	return 0;
}

#endif

