
#include "includes.h"

#ifdef LEDS_FLASH_TASK


#define LEDS_NUM_MAX 43  //�����1-43����43��������43��ʾȫ���ƿ��ƣ�1-42��Ӧĳһ����

static uint8_t g_leds_flash_time[LEDS_NUM_MAX] = {0};  //led ��Ҫ��˸��ʱ��ֵ��0��ʾ500ms��1��ʾ800ms��2��ʾ1s��3��ʾ2s��������ʾ����˸
static uint8_t g_leds_flash_time_already[4] = {0};   //led�Ѿ���˸��ʱ��,ֻ��¼ĳ����˸��ʱ�䣬����ĳһ��led
static uint8_t g_leds_flash_action=0;      // 0-3λ���ֱ��ʾ5��8��10��20�����������1��ʾ����0��ʾ��
static const uint8_t g_const_led_flash_time[4] = {5,8,10,20};




//2023-04-06 ����timer1 �����ж��źţ������Ƽ�����˸
static void TIM1_Led_Pwm_Init(uint16_t arr,uint16_t psc);


//led��˸��ʼ��
void key_light_leds_flash_init(void)
{	
	uint8_t i;
	//����led������˸���ƣ�2022-12-12
	TIM1_Led_Pwm_Init(1000-1,(SystemCoreClock/10000)-1);    //100ms��ʱ��ʼ��

	for(i=0;i<LEDS_NUM_MAX;i++)
	{
		g_leds_flash_time[i] = 5;
	}
}









//����ĳ��led����˸
/*
	whichled :��ʾ�ĸ�����˸��[1-43]  ,ʵ����ЧֵΪ0-42�������м�������			 
	flash_freq :��ʾ��˸������,ֻ����0-3���ֱ��ʾ500ms��800ms��1000ms��2000ms	 
*/
void light_leds_add_flash(uint8_t whichled, uint8_t flash_freq)
{
//	printf("light_leds_add_flash whichled = %d\r\n",whichled);
//	uint8_t flash_freq = whichled >> 6;  
	uint8_t i;

	if(whichled > 43 || whichled == 0)
		return;

	flash_freq &= 0x3;   //ֻ������2λ��
	
	
	whichled = (whichled & 0x3f) - 1;   //����λ��ʾĳ���ƣ�ԭ����ȡֵ��1-43������Ϊ0-42
		
	if(whichled < LEDS_NUM_MAX)
	{	
		timer_enable(TIMER1);	//������˸��ʱ��	
		
		if(whichled == 42)  //(43-1)��ʾȫ���ĵ�
		{		
			for(i=0;i<LEDS_NUM_MAX;i++)
			{
				//led ��Ҫ��˸��ʱ��ֵ��flash_freq:0��ʾ500ms��1��ʾ800ms��2��ʾ1s��3��ʾ2s��������ʾ����˸
				g_leds_flash_time[i] = flash_freq;//g_const_led_flash_time[flash_freq];
			}
			
		}
		else{	
			//led ��Ҫ��˸��ʱ��ֵ��flash_freq:0��ʾ500ms��1��ʾ800ms��2��ʾ1s��3��ʾ2s��������ʾ����˸			
			g_leds_flash_time[whichled] = flash_freq;//g_const_led_flash_time[flash_freq];
		}
	}
}






//����ӿ�������˸�ͳ�������ƣ���ֹͣ��˸����
//whichled: 1-43 ,����42��ʾ����led
//status : 0��ʾϨ�𣬷����ʾ����
void key_light_leds_control2(uint8_t whichled,uint8_t status)
{		
	uint8_t i;
	
	if(whichled > 43 || whichled == 0)
		return;

#ifdef LEDS_FLASH_TASK   //��һ����Ҫ��ȡ���Ƶ���˸����
	if(whichled == 43)
	{
		timer_disable(TIMER1);
		for(i=0;i<LEDS_NUM_MAX;i++)
			g_leds_flash_time[i] = 5;  //ȫ��ȡ����˸
	//	g_leds_flash_control = 0;  //ȫ������Ҫ��˸�ˡ�
		
	}
	else
		g_leds_flash_time[whichled-1] = 5;  //0-3��ʾ������˸ 
#endif	
	cmd_led_control(whichled, (LED_control_t)!!status);
}





#if 1

//2023-04-06 ����timer1 �����ж��źţ������Ƽ�����˸
static void TIM1_Led_Pwm_Init(uint16_t arr,uint16_t psc)
{
	timer_parameter_struct initpara;
	//���ղ���
	rcu_periph_clock_enable(RCU_TIMER1);  //��ʱ��ģ��ʱ��ʹ��

	//3. ��ʼ����ʱ�������ݽṹ  /* initialize TIMER init parameter struct */
	timer_struct_para_init(&initpara);
	initpara.period = arr;  //���ص����֣�
	initpara.prescaler = psc;  //Ԥ��Ƶ�����õ���1Mhz����
	//4. ��ʼ����ʱ��      /* initialize TIMER counter */
	timer_init(TIMER1, &initpara);

	timer_disable(TIMER1);
	timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);	
	
	nvic_irq_enable(TIMER1_IRQn, 7U, 0U);  //����ֻ����ռ���ȼ��ˡ�
	timer_interrupt_enable(TIMER1, TIMER_INT_UP);   //��ʱ�ж�		
}


//��ʱ��1���жϴ���ÿ����1��100ms
void TIMER1_IRQHandler(void)
{
//	static uint8_t count = 0;
	uint8_t i;
	uint8_t c = 0;	
	uint8_t stat[4] = {0};

		
	if(timer_interrupt_flag_get(TIMER1,TIMER_INT_FLAG_UP)!=RESET)
	{	
		timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);
			
		//ledҪ��Ҫ��ת
		stat[0] = g_leds_flash_action & 1;  //500ms����˸��Ҫ��ת
		stat[1] = g_leds_flash_action & 2;   //0 ���߷�0 ��800ms����˸��Ҫ��ת
		stat[2] = g_leds_flash_action & 4;   //1000ms����Ҫ��ת
		stat[3] = g_leds_flash_action & 8;   //2000ms����Ҫ��ת
		
		
		if(g_leds_flash_time[42] < 4 )  //��ʾȫ����Ҫ��
		{
			//ĳ����˸��ʱ����Ҫ��ת��
			if( g_leds_flash_time_already[g_leds_flash_time[42]] == 0)  //���˾���Ҫ��תһ��
			{
				//��ĳ��led���е�������Ϩ�����
				cmd_led_control(43, (LED_control_t)!!stat[g_leds_flash_time[42]]);	 //	cmd_led_control ��һ������Ҫ��1-43��0Ϊ��Чֵ
				//key_light_leds_control2(i,stat[g_leds_flash_time[i]]); //���� or Ϩ��	
			//	Delay1us(50);					
			}
		}
		else
		{
			//ѭ��ɨ�����е�led,ѭ��0-41��������42
			for(i=0;i<LEDS_NUM_MAX-2;i++)  
			{
				if(g_leds_flash_time[i] < 4)  //ĳ������Ҫ��˸�����ڵ���5�Ͳ���Ҫ��˸
				{
					//ĳ����˸��ʱ����Ҫ��ת��
					if( g_leds_flash_time_already[g_leds_flash_time[i]] == 0)  //���˾���Ҫ��תһ��
					{
						//��ĳ��led���е�������Ϩ�����
						cmd_led_control(i+1, (LED_control_t)!!stat[g_leds_flash_time[i]]);	 //	cmd_led_control ��һ������Ҫ��1-43��0Ϊ��Чֵ
						//key_light_leds_control2(i,stat[g_leds_flash_time[i]]); //���� or Ϩ��	
					//	Delay1us(50);					
					}
				}
				else
					c ++;   //��һ�»��ж��ٵƲ���Ҫ��˸
			}
		}
				
		for(i=0;i<4;i++)
		{
			g_leds_flash_time_already[i]++;
			if(g_leds_flash_time_already[i] >= g_const_led_flash_time[i])
			{
				g_leds_flash_time_already[i] = 0;   //�Ѿ���˸��ʱ��
				g_leds_flash_action ^= 1<<i;    //״̬ȡ������ʾled״̬��ת
			}
		}
		//vTaskDelay(pdMS_TO_TICKS(100));  //��ʱ  pdMS_TO_TICKS(10)
		
		if(c == LEDS_NUM_MAX-1)
			timer_disable(TIMER1);  //�رն�ʱ��//g_leds_flash_control = 0;   //û�е���Ҫ��˸�ˡ�

		
	}
//	timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);	
}
#endif


#endif



