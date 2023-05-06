
#include "includes.h"


/*
	键盘扫描处理
*/


// 6*6 的键盘矩阵，总共有33个按键，按键个数在h文件中定义
static BTN_INFO g_btns_info;
uint8_t g_GPIO_KEY_map_value[36];/*按键映射*/




//返回按键1-13的状态，低13位有效，0表示按下，1表示松开
//PC3-PC15
static uint16_t get_keys1_13_states(void)
{
	uint16_t value;
	value = gpio_input_port_get(GPIOC);
	
	return (value >> 3);	
}

//PD0-PD15
//返回按键14-29的状态，0表示按下，1表示松开
static uint16_t get_keys14_29_states(void)
{
	uint16_t value;
	value = gpio_input_port_get(GPIOD);
	
	return (value);	
}

//PE0-PE3
//返回按键30-33的状态，低4位有效，0表示按下，1表示松开
static uint16_t get_keys30_33_states(void)
{
	uint16_t value;
	value = gpio_input_port_get(GPIOE);
	
	return (value & 0xf);		
}



//start_keyn 起始键值1-13，14-29，30-33,从1开始
//end_keyn 结束键值13，29，33
void scan_keys_alone(uint8_t start_keyn,uint8_t end_keyn,uint16_t (*func)(void))
{
	uint8_t index, n = end_keyn-start_keyn+1;
	uint16_t key_value;
//	uint8_t i;
	key_value = func();
	
	start_keyn -= 1;  //换成数组下标	
	
//	printf("start_keyn = %d\n",start_keyn);
	
//	for(i=0;i<33;i++)
//	{
//		printf("g_GPIO_KEY_map_value[%d] = %#x\r\n",i,g_GPIO_KEY_map_value[i]);
//	}
	
	
	for(index=0;index<n;index++)   //1-13
	{
		if(!((key_value>>index)&1))  //某个键被按下,低电平
		{
			printf("value = %#x\r\n",key_value);
			if(!g_btns_info.value[index+start_keyn])
			{	
				g_btns_info.pressCnt[index+start_keyn]++;
				if(g_btns_info.pressCnt[index+start_keyn] == 2)//检测到不止1次
				{   //条件限制上报一次
					g_btns_info.value[index+start_keyn] = 1;
				//	g_btns_info.reportEn[index] = 1;  //按下上报
				//	send_btn_change_to_cpu(index+1,1); //发送按键按下/松开
					fill_tx_data(0x01, g_GPIO_KEY_map_value[index+start_keyn], 0x00, 0x00);
				//	if(more_debug_info)
						printf("@#@#btn:%d press\r\n",index+start_keyn+1);
				//	release_report = 1;   //记录需要释放标志
				}
			}
		}
		else if(g_btns_info.value[index+start_keyn]) //之前的状态是按下
		{								
			g_btns_info.value[index+start_keyn] = 0;
		//	g_btns_info.reportEn[index] = 2;   //松开上报
		//	send_btn_change_to_cpu(index+1,0); //发送按键按下/松开
			fill_tx_data(0x02, g_GPIO_KEY_map_value[index+start_keyn], 0x00, 0x00);
		//	if(more_debug_info)
				printf("@@@btn:%d release\r\n",index+start_keyn+1);
			g_btns_info.pressCnt[index+start_keyn] = 0;			
		}
	}
}





//void keys_scan_task(void)
//{
////	uint8_t i;
//	
//	scan_keys_alone(1,13,get_keys1_13_states);
//	scan_keys_alone(14,29,get_keys14_29_states);
//	scan_keys_alone(30,33,get_keys30_33_states);
//	
//}



#if 0


#endif



