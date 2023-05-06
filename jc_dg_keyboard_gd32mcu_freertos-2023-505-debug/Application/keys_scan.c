
#include "includes.h"


/*
	����ɨ�账��
*/


// 6*6 �ļ��̾����ܹ���33������������������h�ļ��ж���
static BTN_INFO g_btns_info;
uint8_t g_GPIO_KEY_map_value[36];/*����ӳ��*/




//���ذ���1-13��״̬����13λ��Ч��0��ʾ���£�1��ʾ�ɿ�
//PC3-PC15
static uint16_t get_keys1_13_states(void)
{
	uint16_t value;
	value = gpio_input_port_get(GPIOC);
	
	return (value >> 3);	
}

//PD0-PD15
//���ذ���14-29��״̬��0��ʾ���£�1��ʾ�ɿ�
static uint16_t get_keys14_29_states(void)
{
	uint16_t value;
	value = gpio_input_port_get(GPIOD);
	
	return (value);	
}

//PE0-PE3
//���ذ���30-33��״̬����4λ��Ч��0��ʾ���£�1��ʾ�ɿ�
static uint16_t get_keys30_33_states(void)
{
	uint16_t value;
	value = gpio_input_port_get(GPIOE);
	
	return (value & 0xf);		
}



//start_keyn ��ʼ��ֵ1-13��14-29��30-33,��1��ʼ
//end_keyn ������ֵ13��29��33
void scan_keys_alone(uint8_t start_keyn,uint8_t end_keyn,uint16_t (*func)(void))
{
	uint8_t index, n = end_keyn-start_keyn+1;
	uint16_t key_value;
//	uint8_t i;
	key_value = func();
	
	start_keyn -= 1;  //���������±�	
	
//	printf("start_keyn = %d\n",start_keyn);
	
//	for(i=0;i<33;i++)
//	{
//		printf("g_GPIO_KEY_map_value[%d] = %#x\r\n",i,g_GPIO_KEY_map_value[i]);
//	}
	
	
	for(index=0;index<n;index++)   //1-13
	{
		if(!((key_value>>index)&1))  //ĳ����������,�͵�ƽ
		{
			printf("value = %#x\r\n",key_value);
			if(!g_btns_info.value[index+start_keyn])
			{	
				g_btns_info.pressCnt[index+start_keyn]++;
				if(g_btns_info.pressCnt[index+start_keyn] == 2)//��⵽��ֹ1��
				{   //���������ϱ�һ��
					g_btns_info.value[index+start_keyn] = 1;
				//	g_btns_info.reportEn[index] = 1;  //�����ϱ�
				//	send_btn_change_to_cpu(index+1,1); //���Ͱ�������/�ɿ�
					fill_tx_data(0x01, g_GPIO_KEY_map_value[index+start_keyn], 0x00, 0x00);
				//	if(more_debug_info)
						printf("@#@#btn:%d press\r\n",index+start_keyn+1);
				//	release_report = 1;   //��¼��Ҫ�ͷű�־
				}
			}
		}
		else if(g_btns_info.value[index+start_keyn]) //֮ǰ��״̬�ǰ���
		{								
			g_btns_info.value[index+start_keyn] = 0;
		//	g_btns_info.reportEn[index] = 2;   //�ɿ��ϱ�
		//	send_btn_change_to_cpu(index+1,0); //���Ͱ�������/�ɿ�
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



