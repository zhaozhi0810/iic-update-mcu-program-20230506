
/*
���ڴ�����cpu֮��Ĵ���ͨ��
	����ΪGD32�Ĵ���1��115200��8N1


���Դ��ڽ��յ����
0.�������ʱ���ַ���
1.��ѹ����
2.cpu�������¶ȣ�Һ�����¶ȣ���lcd����״̬������pwmֵ��ioģʽ��ֻ��0��100��
3.lcd������ֵ�����ļӵ�����״̬��pd_n��״̬
4.4·diֵ��4·��ͨ·��Ϣ
5.Ӳ�����Ź�״̬�����ź�Դ�����ޣ�
6.cpu����״̬���������ػ�������������pmon������ϵͳ�ȣ�

����ʶ�������Ҳ�Ǵ�ӡ��ʾ�ͱ���ʱ���ַ���
*/



#include "includes.h"



//#define RECV_BUF_LEN 64
#if 0
static Queue_UART_STRUCT g_Queue_Debug_Recv;   //����Debug���ݶ��У����ڽ����ж�

frame_buf_t g_com_debug_buf={{0},FRAME_LENGHT};    //���ݴ�����
#endif

//static char* g_Cpu_Run_Status_str[] = {
//	"LS3A_POWER_DISABLE",   //ȷ���Ƕϵ�״̬
//	"LS3A_POWEROFF",    //�ػ����ϵ�
//	"LS3A_REBOOT",    //����
//	"LS3A_RUNNING",    //����pmon
//	"LS3A_RUN_OS",      //�뿪pmon���������ϵͳ
//	"LS3A_POWER_ENABLE"     //�Ѿ�ͨ�磬����û�н���PMON��ǰһ��;
//};

static void  Com_Debug_Print_Help(void)
{
	printf("\r\nDebug cmd:\r\n");
	printf("0. print Program build time\r\n");

	printf("1. key_LEDs_lightAll\r\n");
	printf("2. key_LEDs_OffAll\r\n");

	printf("3. key_LEDs set PWM ++++ \r\n");
	printf("4. key_LEDs set PWM ---- \r\n");
//	printf("5. print Hard Watch Dog Status\r\n");
//	printf("6. print Mcu internal_temp\r\n");
	printf("other. print help\r\n");
}


static void print_md5sum(void)
{
	char buf[36] = {0};
	int i;
	uint8_t *pdown_md5 = (void*)(UPDATE_FLAG_START_ADDR + DOWN_MD5_OFFET);
	
	for(i=0;i<32;i++)
	{
		buf[i] = pdown_md5[i];
	}
	
	printf("md5: %s\r\n",buf);
}




//�����������������Դ��ڽ��յ��ļ򵥵ĵ�������
static void Com_Debug_Message_Handle1(uint8_t buf)
{
	uint8_t t;
//	uint8_t t;
	switch(buf)
	{
		default:   //cmd��ӡ��ʱ�򣬿��ܳ����˿���ʾ�ַ�������
			printf("ERROR: Command Unknow cmd = 0x%x!!!\r\n",buf);   //����ʶ�������
			Com_Debug_Print_Help();
		case '0':
			printf("*FreeRTOS* ");
			printf("%s\r\n",(char*)g_build_time_str);  //��ӡ�����ʱ��
			printf("Author:JC&DaZhi <vx:285408136>\r\n"); 
			printf("Version : %02d\r\n",SOFT_VERSION); 
			print_md5sum();
		break;
		case '1':
			printf("AT9236_LED_lightAll\r\n");  //lcd�ӵ�״̬
			AT9236_LED_lightAll();
			break;
		case '2':
			printf("AT9236_LED_lightAll_OFF\r\n");  //lcd�ӵ�״̬
			AT9236_LED_lightAll_OFF();
			break;
		case '3':
			t = get_keyleds_pwm();
			if(t < 100)
				t += 5;
			printf("set_keyleds_pwm +++ %d\r\n",t);  //lcd�ӵ�״̬
			set_keyleds_pwm(t);

			break;
		case '4':
			t = get_keyleds_pwm();
			if(t >= 5)
				t -= 5;
			printf("set_keyleds_pwm --- %d\r\n",t);  //lcd�ӵ�״̬
			set_keyleds_pwm(t);
			break;
	}
}


/*
	�������ݽ����жϣ�
		ǰ�᣺ÿһ֡����7���ֽڡ�
		�����б���֡ͷ���к�������ݺ�У��ͣ���7���ֽڣ�
*/
void Com_Debug_Rne_Int_Handle(void)
{
	uint8_t dat;

	dat = (uint8_t)usart_data_receive(EVAL_COM0);//(USART3);  
	Com_Debug_Message_Handle1(dat);   //ֱ�Ӵ���
//	QueueUARTDataInsert(&g_Queue_Debug_Recv,dat);   //���յ����ݴ�������С�
}


/*
	�����յ������Ĵ������ڵĿ����жϴ���������
		ǰ�᣺ �յ����������ݰ���У�����ȷ��

	��Ƭ���ܹ��յ������
	// 1.������ƵԴ,û�иù���
	4.����lcd��pwm�����ȣ�
	5.�ػ����������

*/

#if 0

static void Com_Debug_Message_Handle(uint8_t* buf)
{		
	com_frame_t* pdata = (com_frame_t*)(buf+1);    //+1������֡ͷ��ʹ�ýṹ���ʼ��
	int8_t t;
	
	switch(pdata->data_type)
    {
        case eMCU_CMD_TYPE:    //cpu���͸���Ƭ���Ķ���cmd����
            t = pdata->data.cmd.cmd;
            switch(t)
            {
				case eMCU_CPUGETINFO_CMD:   //��ȡ�豸��Ϣ������
				//	AnswerCpu_GetInfo(pdata->data.cmd.param1<<8 | pdata->data.cmd.param2); //ʹ�ú�������������������
					break;
				case eMCU_CPUSET_CMD:    //������Ļ����
					if(pdata->data.cmd.param1 == eMCU_LCD_SETPWM_CMD)
					{
						t = pdata->data.cmd.param2;   //���ֵ�����ɸ���������������������������
						t = g_lcd_pwm + t;   //����ó��µĽ��
						Lcd_pwm_out(t);     //��������pwm��ֵ
				//		AnswerCpu_Status(eUART_SUCCESS);   //Ӧ��ɹ�
					}
					else if(pdata->data.cmd.param1 == eMCU_SWITCH_DVI_SRC_CMD) //�л���ƵԴ
					{
						t = pdata->data.cmd.param2;  //0 Ϊ���أ�1Ϊ�ⲿ
//						if(t)
//							dvi_switch_set(DVI_OTHER);   //���ú���ϱ���cpu
//						else
//							dvi_switch_set(DVI_LOONGSON);   //������Ƶ
				//		AnswerCpu_Status(eUART_SUCCESS);   //Ӧ��ɹ�
					}
					else	
				//		AnswerCpu_Status(eUART_ERR_PARAM);  //Ӧ���������				
				break;
                default:
					DBG_PRINTF("ERROR: %s\n","eUART_ERR_PARAM");
				//	AnswerCpu_Status(eUART_ERR_PARAM);  //Ӧ���������
                break;
            }

        break;
        default:
			DBG_PRINTF("ERROR: %s\n","eUART_ERR_CMD_UNKNOW");
		//	AnswerCpu_Status(eUART_ERR_CMD_UNKNOW);  //Ӧ������δ֪ 
        break;
    }	
}
#endif



/*
	���ڿ����жϵĴ���,���Դ��ڲ��ٿ��������ж�

	1.�жϽ��յ����ֽ�����>=7 ��ʾ����
	2.�����ͼ�����������7���ֽڣ�����У��ͣ�
	3.У�����ȷ����������
*/
void Com_Debug_Idle_Int_Handle(void)
{
//	Com_Frame_Handle(&g_com_debug_buf, &g_Queue_Debug_Recv,Com_Debug_Message_Handle);
}


