/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：iic.c
* 摘要：自定义iic配置
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/
#include "includes.h"
//#include "soft_iic_master_app.h"
//#include "software_iic_slave.h"


#define I2C0_SLAVE_ADDRESS7     0x80
#define I2C1_SLAVE_ADDRESS7     0xAC
#define I2C0_SPEED              100000
#define I2C1_SPEED              100000



#if 0
void IIc_dey(short us)
{
    int i, j;
    for (i = 0; i < us; i++)
        for (j = 0; j < 10; j++);
}

/*初始化IIC0*/
void CH453_IIC_Init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure led GPIO port */
    gpio_init(CH453_IO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CH453_SCL_IO_PIN);
    gpio_init(CH453_IO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CH453_SDA_IO_PIN);
    GPIO_BC(CH453_IO_PORT) = CH453_SCL_IO_PIN | CH453_SDA_IO_PIN;
    IIC_SCL_SET;
    IIC_SDA_SET;
}

/*产生IIC起始信号*/
void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA_SET;
    IIC_SCL_SET;
    IIc_dey(2);
    IIC_SDA_RESET;//START:when CLK is high,DATA change form high to low
    IIc_dey(2);
    IIC_SCL_RESET;//钳住I2C总线，准备发送或接收数据
}

/*产生IIC停止信号*/
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL_RESET;
    IIC_SDA_RESET;//STOP:when CLK is high DATA change form low to high
    IIc_dey(2);
    IIC_SCL_SET;
    IIc_dey(2);
    IIC_SDA_SET;//发送I2C总线结束信号
    IIc_dey(2);
}

/*
等待应答信号到来
返回值：1，接收应答失败
        0，接收应答成功
*/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();      //SDA设置为输入
    IIC_SDA_SET;
    IIc_dey(4);//delay_us(1);
    IIC_SCL_SET;
    IIc_dey(4);//delay_us(1);
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_RESET;//时钟输出0
    return 0;
}

/*产生ACK应答*/
void IIC_Ack(void)
{
    IIC_SCL_RESET;
    SDA_OUT();
    IIC_SDA_RESET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_SET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_RESET;
}

/*不产生ACK应答*/
void IIC_NAck(void)
{
    IIC_SCL_RESET;
    SDA_OUT();
    IIC_SDA_SET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_SET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_RESET;
}

/*
IIC发送一个字节
返回从机有无应答
1，有应答
0，无应答
*/
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL_RESET;//拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        uint8_t i = (txd & 0x80) >> 7;
        if (i)
        {
            IIC_SDA_SET;
        }
        else
        {
            IIC_SDA_RESET;
        }
        txd <<= 1;
        IIc_dey(2);
        IIC_SCL_SET;
        IIc_dey(2);
        IIC_SCL_RESET;
    }
    SDA_IN();      //SDA设置为输入
    IIc_dey(2);
    IIC_SCL_SET;
    IIc_dey(2);
    IIC_SCL_RESET;
}

/*读1个字节，ack=1时，发送ACK，ack=0，发送nACK */
uint8_t IIC_Read_Byte()
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        IIC_SCL_RESET;
        IIc_dey(2);
        IIC_SCL_SET;
        receive <<= 1;
        if (READ_SDA) receive++;
        IIc_dey(2);
    }
    return receive;
}

/*写命令*/
void CH453_Write(uint16_t cmd)
{
    IIC_Start();
    IIC_Send_Byte((uint8_t)(cmd >> 8));
    IIC_Send_Byte((uint8_t)cmd);         /*发送数据*/
    IIC_Stop();
}

#endif


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
    \brief      cofigure the GPIO ports.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC1_gpio_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* I2C1 GPIO ports */
    /* connect PB10 to I2C1_SCL */
    /* connect PB11 to I2C1_SDA */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);
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

/*!
    \brief      cofigure the I2C0 and I2C1 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC1_config(void)
{
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);
    /* configure I2C1 clock */
    i2c_clock_config(I2C1, I2C1_SPEED, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

/*!
    \brief      cofigure the NVIC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC1_IRQ_config(void)
{
    nvic_irq_enable(I2C1_EV_IRQn, 0, 1);
    nvic_irq_enable(I2C1_ER_IRQn, 0, 1);
    /* enable the I2C1 interrupt */
    i2c_interrupt_enable(I2C1, I2C_INT_ERR);
    i2c_interrupt_enable(I2C1, I2C_INT_EV);
    i2c_interrupt_enable(I2C1, I2C_INT_BUF);
}

/*iic初始化*/
void IIC0_init(void)
{	
    IIC0_gpio_config();
    IIC0_config();
    IIC0_IRQ_config();
//	InitSwSlaveI2C0();
}


/*iic初始化*/
void IIC1_init(void)
{	

    IIC1_gpio_config();
    IIC1_config();
	
	CH453_AT9236_map_init();
    AT9236_transmit_byte(0x4f, 0x00);  // reset
    AT9236_transmit_byte(0x00, 0x01);  // set shutdown register(0x00: software shutdown mode; 0x01: normal operation)
    AT9236_transmit_byte(0x4A, 0x00);  // set all channels enable
    AT9236_transmit_byte(0x4B, 0x01);  // Output Frequency Setting 22KHz(0x00:3KHz; 0x01:22KHz)
    
	set_keyleds_pwm(4);   //设置开机后的默认亮度是4%
    AT9236_LED_lightAll();  //点亮全部的灯
	
//	IicApp_Init(IIC2_INDEX);
}



#if 1
/*i2c0数据接收函数*/
uint8_t i2c0_get_txdata(uint8_t *rdata)
{
    uint8_t reallen;
    uint8_t i;
//    uint8_t temp[10];
    reallen = CIRC_RM_CNT(g_i2c0_txbuf);

    if (reallen < 4)   //发送有7个字节，去掉帧头2个字节0x55，0xaa，和末尾的校验和，只剩下4个字节的数据。
    {
        return 0;
    }
	
    for (i = 0; i < 4; i++)//只读4个字节
    {
        CIRC_GET_CH(g_i2c0_txbuf, rdata[i]);
    }

    return (4);
}




/*填充要发送的数据*/
void iic0_send_data(uint8_t *buf)
{
//	g_IIC_tx_buf[0] = 0x55;
//	g_IIC_tx_buf[1] = 0xaa;
	g_IIC_tx_buf[2] = buf[0];
    g_IIC_tx_buf[3] = buf[1];
    g_IIC_tx_buf[4] = buf[2];
    g_IIC_tx_buf[5] = buf[3];
    g_IIC_tx_buf[6] = CheckSum((void*)g_IIC_tx_buf, 7);
	
	gpio_bit_reset(GPIOA, GPIO_PIN_2); //high->low
    gpio_bit_set(GPIOA, GPIO_PIN_2); //low->high
	
}


/*任务5  把iic缓存的东西发出去
*/
void iic0_send_data_task(void *pdata)
{
    uint8_t tx_data[8] = {0};

    while (1)
    {
		ulTaskNotifyTake(ULONG_MAX,  //退出时，清除对应的位，0表示都不清零
						portMAX_DELAY); //无限等待
//		vTaskDelay(10);
		//等待事件
//		xEventGroupWaitBits(g_xeventGh ,  
//						  0x2,     //bit1
//						 pdTRUE ,   //退出时清除该位？
//						 pdFALSE,	 //或，只等1个位	
//						 portMAX_DELAY);
		
		//printf("------iic0_send_data_task \r\n");
        while (i2c0_get_txdata(tx_data) > 0)
        {
			//printf("iic0_send_data_task txv2 = %#x,txv3 = %#x,txv4 = %#x,txv5 = %#x\r\n",tx_data[0],tx_data[1],tx_data[2],tx_data[3]);
			for(;;)
			{
			//	printf("iic0_2023-03-16 \r\n");
				if(g_iic0_is_Idle)  //确认是空闲状态
				{
					g_iic0_is_Idle = 0;
					iic0_send_data(tx_data);   //发送数据
				//	g_iic0_has_senddatas = 1;  //数据准备好了
				//	xSemaphoreGive(iic0_has_senddatas);
					break;
				}
				else
					vTaskDelay(10);
			}
				
        }
    }
}

#endif

