
#include "includes.h"

#ifdef LEDS_FLASH_TASK


#define LEDS_NUM_MAX 43  //最多是1-43，共43个，其中43表示全部灯控制，1-42对应某一个灯

static uint8_t g_leds_flash_time[LEDS_NUM_MAX] = {0};  //led 需要闪烁的时间值，0表示500ms，1表示800ms，2表示1s，3表示2s，其他表示不闪烁
static uint8_t g_leds_flash_time_already[4] = {0};   //led已经闪烁的时间,只记录某种闪烁的时间，不是某一个led
static uint8_t g_leds_flash_action=0;      // 0-3位，分别表示5，8，10，20的亮灭情况，1表示亮，0表示灭
static const uint8_t g_const_led_flash_time[4] = {5,8,10,20};




//2023-04-06 改由timer1 产生中断信号，来控制键灯闪烁
static void TIM1_Led_Pwm_Init(uint16_t arr,uint16_t psc);


//led闪烁初始化
void key_light_leds_flash_init(void)
{	
	uint8_t i;
	//用于led键灯闪烁控制，2022-12-12
	TIM1_Led_Pwm_Init(1000-1,(SystemCoreClock/10000)-1);    //100ms定时初始化

	for(i=0;i<LEDS_NUM_MAX;i++)
	{
		g_leds_flash_time[i] = 5;
	}
}









//增加某个led灯闪烁
/*
	whichled :表示哪个灯闪烁，[1-43]  ,实际有效值为0-42，函数中间做调整			 
	flash_freq :表示闪烁的速率,只能是0-3，分别表示500ms，800ms，1000ms，2000ms	 
*/
void light_leds_add_flash(uint8_t whichled, uint8_t flash_freq)
{
//	printf("light_leds_add_flash whichled = %d\r\n",whichled);
//	uint8_t flash_freq = whichled >> 6;  
	uint8_t i;

	if(whichled > 43 || whichled == 0)
		return;

	flash_freq &= 0x3;   //只保留低2位。
	
	
	whichled = (whichled & 0x3f) - 1;   //低六位表示某个灯，原来的取值是1-43，调整为0-42
		
	if(whichled < LEDS_NUM_MAX)
	{	
		timer_enable(TIMER1);	//开启闪烁定时器	
		
		if(whichled == 42)  //(43-1)表示全部的灯
		{		
			for(i=0;i<LEDS_NUM_MAX;i++)
			{
				//led 需要闪烁的时间值，flash_freq:0表示500ms，1表示800ms，2表示1s，3表示2s，其他表示不闪烁
				g_leds_flash_time[i] = flash_freq;//g_const_led_flash_time[flash_freq];
			}
			
		}
		else{	
			//led 需要闪烁的时间值，flash_freq:0表示500ms，1表示800ms，2表示1s，3表示2s，其他表示不闪烁			
			g_leds_flash_time[whichled] = flash_freq;//g_const_led_flash_time[flash_freq];
		}
	}
}






//这个接口用于闪烁和常亮灭控制，会停止闪烁控制
//whichled: 1-43 ,其中42表示所有led
//status : 0表示熄灭，非零表示点亮
void key_light_leds_control2(uint8_t whichled,uint8_t status)
{		
	uint8_t i;
	
	if(whichled > 43 || whichled == 0)
		return;

#ifdef LEDS_FLASH_TASK   //这一段主要是取消灯的闪烁控制
	if(whichled == 43)
	{
		timer_disable(TIMER1);
		for(i=0;i<LEDS_NUM_MAX;i++)
			g_leds_flash_time[i] = 5;  //全部取消闪烁
	//	g_leds_flash_control = 0;  //全部不需要闪烁了。
		
	}
	else
		g_leds_flash_time[whichled-1] = 5;  //0-3表示设置闪烁 
#endif	
	cmd_led_control(whichled, (LED_control_t)!!status);
}





#if 1

//2023-04-06 改由timer1 产生中断信号，来控制键灯闪烁
static void TIM1_Led_Pwm_Init(uint16_t arr,uint16_t psc)
{
	timer_parameter_struct initpara;
	//接收部分
	rcu_periph_clock_enable(RCU_TIMER1);  //定时器模块时钟使能

	//3. 初始化定时器的数据结构  /* initialize TIMER init parameter struct */
	timer_struct_para_init(&initpara);
	initpara.period = arr;  //重载的数字，
	initpara.prescaler = psc;  //预分频数，得到是1Mhz的脉
	//4. 初始化定时器      /* initialize TIMER counter */
	timer_init(TIMER1, &initpara);

	timer_disable(TIMER1);
	timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);	
	
	nvic_irq_enable(TIMER1_IRQn, 7U, 0U);  //现在只有抢占优先级了。
	timer_interrupt_enable(TIMER1, TIMER_INT_UP);   //定时中断		
}


//定时器1的中断处理，每进入1次100ms
void TIMER1_IRQHandler(void)
{
//	static uint8_t count = 0;
	uint8_t i;
	uint8_t c = 0;	
	uint8_t stat[4] = {0};

		
	if(timer_interrupt_flag_get(TIMER1,TIMER_INT_FLAG_UP)!=RESET)
	{	
		timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);
			
		//led要不要翻转
		stat[0] = g_leds_flash_action & 1;  //500ms的闪烁需要翻转
		stat[1] = g_leds_flash_action & 2;   //0 或者非0 ，800ms的闪烁需要翻转
		stat[2] = g_leds_flash_action & 4;   //1000ms的需要翻转
		stat[3] = g_leds_flash_action & 8;   //2000ms的需要翻转
		
		
		if(g_leds_flash_time[42] < 4 )  //表示全部的要闪
		{
			//某种闪烁的时间需要翻转吗？
			if( g_leds_flash_time_already[g_leds_flash_time[42]] == 0)  //到了就需要翻转一次
			{
				//对某个led进行点亮或者熄灭控制
				cmd_led_control(43, (LED_control_t)!!stat[g_leds_flash_time[42]]);	 //	cmd_led_control 第一个参数要求1-43，0为无效值
				//key_light_leds_control2(i,stat[g_leds_flash_time[i]]); //点亮 or 熄灭	
			//	Delay1us(50);					
			}
		}
		else
		{
			//循环扫描所有的led,循环0-41，不包括42
			for(i=0;i<LEDS_NUM_MAX-2;i++)  
			{
				if(g_leds_flash_time[i] < 4)  //某个灯需要闪烁，大于等于5就不需要闪烁
				{
					//某种闪烁的时间需要翻转吗？
					if( g_leds_flash_time_already[g_leds_flash_time[i]] == 0)  //到了就需要翻转一次
					{
						//对某个led进行点亮或者熄灭控制
						cmd_led_control(i+1, (LED_control_t)!!stat[g_leds_flash_time[i]]);	 //	cmd_led_control 第一个参数要求1-43，0为无效值
						//key_light_leds_control2(i,stat[g_leds_flash_time[i]]); //点亮 or 熄灭	
					//	Delay1us(50);					
					}
				}
				else
					c ++;   //数一下还有多少灯不需要闪烁
			}
		}
				
		for(i=0;i<4;i++)
		{
			g_leds_flash_time_already[i]++;
			if(g_leds_flash_time_already[i] >= g_const_led_flash_time[i])
			{
				g_leds_flash_time_already[i] = 0;   //已经闪烁的时间
				g_leds_flash_action ^= 1<<i;    //状态取反，表示led状态翻转
			}
		}
		//vTaskDelay(pdMS_TO_TICKS(100));  //延时  pdMS_TO_TICKS(10)
		
		if(c == LEDS_NUM_MAX-1)
			timer_disable(TIMER1);  //关闭定时器//g_leds_flash_control = 0;   //没有灯需要闪烁了。

		
	}
//	timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);	
}
#endif


#endif



