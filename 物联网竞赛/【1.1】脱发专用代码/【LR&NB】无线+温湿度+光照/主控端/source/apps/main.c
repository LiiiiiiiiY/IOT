/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
#include <string.h>
#include "board.h"
#include "hal_key.h"
#include "tim-board.h"
#include "timer_handles.h"
#include "hal_oled.h"//oled��
#include "adc_reader.h"//����
#include "sht3x.h"//��ʪ��
#include "NS_Radio.h"
#include "usart1-board.h"
/**********************************************************************************************
*������void Init( void )
*���ܣ�ƽ̨��ʼ��
*���룺��
*�������
*����˵������
**********************************************************************************************/
void Init() {
    // ������ƽ̨��ʼ��
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//������ʼ��
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��
	 
}

/**********************************************************************************************
*������void KeyDownHandler( void )
*���ܣ���ť�¼�����
*���룺��
*�������
*����˵������
**********************************************************************************************/
void KeyDownHandler(void) {
	
}

/**********************************************************************************************
*������void handlerPre10Ms( void )
*���ܣ�10����ѭ��
*���룺��
*�������
*����˵����ѭ��������ʱ��300ms
**********************************************************************************************/
void handlerPre10Ms(void) {
    for (int delay = 0; delay < 30; delay++) {
        HAL_Delay(10);
    }
}

void menu()								//������
{
	OLED_ShowString(50,0,(uint8_t *)"Menu");
	OLED_ShowString(1,2,(uint8_t *)"1.Auto Lamp");
	OLED_ShowString(1,4,(uint8_t *)"2.Mula Lamp");
	OLED_ShowString(1,6,(uint8_t *)"3.View Freq");
}

void Auto_Lamp()					//������ʪ�Ƚ���
{
	OLED_ShowString(20,0,(uint8_t *)"Auto Lamp");
	OLED_ShowString(1,2,(uint8_t *)"Light:");
	OLED_ShowString(1,4,(uint8_t *)" Temp:");
	OLED_ShowString(1,6,(uint8_t *)" Humi:");
}

void Mula_Lamp()					//�ƹ���ƽ���
{
	OLED_ShowString(20,0,(uint8_t *)"Mula Lamp");
	OLED_ShowString(1,2,(uint8_t *)"Lamp:");
}
extern unsigned char bitmap4oled[];		//����oledfont.h�ı�����wifiͼ�꣩
void View_Freq()											//�ز�Ƶ�ʽ���
{
	OLED_ShowString(20,0,(uint8_t *)"View Freq");
	OLED_ShowString(1,2,(uint8_t *)"Freq:");
	OLED_ShowNum(48,2,433000000,9,16);	//��ʾ�ز�Ƶ��
	OLED_DrawBMP(1,4,27,24,bitmap4oled);//��ʾwifiͼ
}

uint8_t send[64]={0,1};	//���ڴ�����߷������ݵı���
													//����λ�����жϽ���ı�־λ����0�������Ϊ�����棬1�������Ϊ��ʪ�ȹ��ս����...
													//��һλ�����ж�mula lamp�����led��״̬�������0�ʹ���open��������led�����������1�ʹ���close��������led�أ�
													//�ڶ�λ���ڴ�Ź���ֵ�������ֳ�10������
													//����λ���ڴ�Ź���ֵ�������ֳ�10������
													//����λ���ڴ�Ź���ֵ��С������
													//����λ���ڴ�Ź���ֵ����������
													//����λ���ڴ�Ź���ֵ��С������
													//����λ���ڴ�Ź���ֵ����������
													//�ڰ�λ���ڴ�Ź���ֵ��С������

int pointer_count=1;			//������ļ�ͷ
uint8_t interface_flag=0;	//�жϽ���ı�־λ

void menu_pointer()  			//�ж�������ָ�뺯��
{
	if(isKey2Pressed())			//���Key2���°��£�����ָ�������ƶ���
	{
		OLED_Clear_Page(pointer_count*2);		//�����pointer_count*2�У��磺pointer_count=1���ʹ��������ڶ��У�
		pointer_count--;
		if(pointer_count==0)pointer_count=3;//����жϵ������ǣ����ָ���ڵ�һ��ʱ��������key2���������ð������ڵ�����
		resetKey2();					//����key2������״̬
	}
	else if(isKey3Pressed())//���Key3�������£�����ָ�������ƶ���
	{
		OLED_Clear_Page(pointer_count*2);
		pointer_count++;
		if(pointer_count==4)pointer_count=1;//����жϵ������ǣ����ָ���ڵ�����ʱ��������key3���������ð������ڵ�һ��
		resetKey3();					//����Key3������״̬
	}
	
	OLED_ShowString(96,pointer_count*2,(uint8_t *)"<");//��ʾָ�루����pointer_count�����иı䡢��ʾ��
	
	if(isKey4Pressed())			//���Key4�������£�����ȷ�ϣ�
	{
		
		interface_flag = pointer_count;	//��ָ���ڵڼ��и�ֵ���жϽ���ı�־λ
		send[0] = interface_flag;				//���жϽ���ı�־λ��ֵ�����ͱ����ĵ���λ
		Radio.Send(send,16);						//���ͺ�������send���͸�ͬƵ���󣨿�һ�Զࣩ
		OLED_Clear();										//�����Ļ����
		resetKey4();										//����Key4������״̬
	}
}

float light;				//���ڴ�Ź���ֵ�����ַ�������Ϊ��Ŀ����ʪ�ȹ��ձ�������λС����
float hum;					//���ڴ��ʪ��ֵ
float temp;					//���ڴ���¶�ֵ
int Led_state=1;		//���ڼ�¼led״̬�ı�����1����led�رգ�0����led������

void Show_data()		//�鿴��ʪ�ȹ������ݽ�����ں�
{
	char data[10];		//���ڴ�Ÿ�ʽ���������ֵ
	AdcScanChannel();	//ת��ADC˫�ŵ��Ĵ�������Ϊ��ѹֵ
	light = (5/2.0)*(AdcReadCh0()*100);			//����ֵ�Ĺ̶���ʽ��AdcReadCh0()���ص�ѹֵ������=��5/20��*����ѹֵ*100����
	sprintf(data,"%.2f  ",light);						//�����Ӳ���lightͨ��������λС���ĸ�ʽд��data�д��
	OLED_ShowString(48,2,(uint8_t *)data);	//��oled�����data�е�����
	send[2]	=	(int)light / 10;							//ȡ����ֵ���������֣�����ֵ�����ͱ����ĵڶ�λ
	send[3] = (int)light % 10;							//��10��ȡ������Ϊchar(uint8_t)���ֻ�ܴ浽255,���պ����׳���255Lx
	send[4] = (light-(int)light)*100;				//ȡ����ֵ��С�����֣�����ֵ�����ͱ����ĵ���λ
										//С������=������ֵ-����ֵ���������֣�*100��������>��100����Ϊ���ߴ���ʱ��char(uint8_t)��ֻ�ܴ�������
	if(light<60)						//�������ֵС��60
	{
		GpioWrite(&Led1,0);
		GpioWrite(&Led2,0);		//����Led1��Led2
		Led_state=0;					//��¼Led1��Led2��ǰ״̬��������
		send[1]= Led_state;		//��Led��ǰ״̬��ֵ�����ͱ����ĵ�һλ
	}
	else 										//�������ֵ���ڵ���60
	{
		GpioWrite(&Led1,1);
		GpioWrite(&Led2,1);		//�ر�Led1��Led2
		Led_state=1;					//��¼Led1��Led2��ǰ״̬(�ر�)
		send[1]= Led_state;		//��Led��ǰ״̬����ڷ��ͱ����ĵ�һλ
	}	
	
	call_sht11(&temp,&hum);	//��ȡ��ʪ�ȹ���ֵ������ڱ���temp��hum��
	sprintf(data,"%.2f  ",temp);					//���¶�ֵͨ��������λС���ĸ�ʽд��data��
	OLED_ShowString(48,4,(uint8_t *)data);//��oled�����data�е�����
	send[5]=(int)temp;									//�����¶�ֵ���������֣�����ֵ�����ͱ����ĵ���λ
	send[6]=(temp-(int)temp)*100;						//�����¶�ֵ��С�����֣�����ֵ�����ͱ����ĵ���λ
	USART1_SendStr(send,4);
	sprintf(data,"%.2f  ",hum);						//���¶�ֵͨ��������λС���ĸ�ʽд��data��
	OLED_ShowString(48,6,(uint8_t *)data);//��oled�����data�е�����
	send[7]=(int)hum;											//����ʪ��ֵ���������֣�����ֵ�����ͱ����ĵ���λ
	send[8]=(hum-(int)hum)*100;						//����ʪ��ֵ��С�����֣�����ֵ�����ͱ����ĵ���λ
	
	Radio.Send(send,16);									//���ͺ�������send���͸�ͬƵ���󣨿�һ�Զࣩ
}

void Show_mula()					//����Led״̬���ں�
{
	if(isKey2Pressed())			//���Key2�������£�Led1��Led2����
	{
		GpioWrite(&Led1,0);
		GpioWrite(&Led2,0);
		Led_state=0;					//��¼Led1��Led2��ǰ״̬��������
		send[1]= Led_state;		//��Led��ǰ״̬��ֵ�����ͱ����ĵ�һλ
		Radio.Send(send,16);	//���ͺ�������send���͸�ͬƵ���󣨿�һ�Զࣩ
		resetKey2();					//����Key2������״̬
	}
	if(isKey3Pressed())			//���Key3�������£�Led1��Led2�أ�
	{
		GpioWrite(&Led1,1);
		GpioWrite(&Led2,1);
		Led_state=1;					//��¼Led1��Led2��ǰ״̬���رգ�
		send[1]= Led_state;		//��Led��ǰ״̬��ֵ�����ͱ����ĵ�һλ
		Radio.Send(send,16);	//���ͺ�������send���͸�ͬƵ���󣨿�һ�Զࣩ
		resetKey3();					//����Key3������״̬
	}
	if(Led_state==0)				//���Led״̬Ϊ����
	{
		OLED_ShowString(48,2,(uint8_t *)"Open ");	//��Oled������ʾOpen����һ���ո�����Ϊcolse��һ���ַ������open��ͻ��
	}
	else if(Led_state==1)		//���Led״̬Ϊ�ر�
	{
		OLED_ShowString(48,2,(uint8_t *)"Close");	//��Oled������ʾClose
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();							//������ƽ̨��ʼ��
	OLED_Init();				//Oled����ʼ��
	ADCS_Init();				//ADCͨ��0��ͨ��2��ʼ��
	hal_temHumInit();		//��ʪ��ģ���ʼ��
	NS_RadioInit(433000000,20,1000,1000);	//���߳�ʼ��
	send[0]=0;					//�����帴λ��send����λ��0��ͬƵ�����彫�ص������棩
	Radio.Send(send,16);//���ͺ�������send���͸�ͬƵ���󣨿�һ�Զࣩ
	USART1_Init(115200);
  while(1)
  {
		if(interface_flag==0)					//�������־λΪ0ʱ����ʾ���������ݼ�ָ��
		{
			menu();					//����
			menu_pointer();	//ָ��
		}
		else if(interface_flag==1)		//�������־λΪ1ʱ����ʾ��ʪ�ȹ��ս��������
		{
			Auto_Lamp();		//��ʪ�Ƚ���
			Show_data();		//��ʪ�ȹ�������
			if(isKey4Pressed())					//�������4���£����������Ϣ������������
			{
				interface_flag=0;					//�����־λ-��0
				send[0] = interface_flag;	//�����־λ��ֵ�����ͱ����ĵ���λ
				Radio.Send(send,16);			//���ͺ�������send���͸�ͬƵ���󣨿�һ�Զࣩ
				OLED_Clear();	//����
				resetKey4();	//����Key4״̬
			}
		}
		else if(interface_flag==2)		//�������־λΪ2ʱ����ʾled״̬���漰�����ں�
		{
			Mula_Lamp();		//״̬����
			Show_mula();		//״̬�ں�
			if(isKey4Pressed())					//�������4���£����������Ϣ������������
			{
				interface_flag=0;
				send[0] = interface_flag;
				Radio.Send(send,16);
				OLED_Clear();
				resetKey4();
			}
		}
		else if(interface_flag==3)		//�������־λΪ3ʱ����ʾ�ز�Ƶ�ʼ�WiFiͼ��
		{
			View_Freq();		//�ز�Ƶ�ʼ�ͼ��
			if(isKey4Pressed())					//�������4���£����������Ϣ������������
			{
				interface_flag=0;
				send[0] = interface_flag;
				Radio.Send(send,16);
				OLED_Clear();
				resetKey4();
			}
		}
  }
}
