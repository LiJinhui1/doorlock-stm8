#include "stm8l15x.h"
//����LED�������˿�
#define LED_PORT        GPIOD
#define LED_PINS        GPIO_Pin_0
#define ADC_1PORT       GPIOC
#define ADC_1PINS       GPIO_Pin_4
#define ADC_2PORT       GPIOB
#define ADC_2PINS       GPIO_Pin_0
#define LED_TOGGLE      GPIO_ToggleBits(LED_PORT,LED_PINS)
#define LED_OFF         GPIO_SetBits(LED_PORT,LED_PINS)
#define LED_ON          GPIO_ResetBits(LED_PORT,LED_PINS)
#define MAX_DELAY       0xFFFFU
#define SINGLEBUS_PORT       GPIOB
#define SINGLEBUS_PINS       GPIO_Pin_1
#define PRESTART_EDGE        0x1067     //����ж���(4199+1)us
#define RIGHT_EDGE           0xB3       // ����ݴ���180us��(179+1)
#define LEFT_EDGE            0x3B         //��С�ı߽���60us(59+1) ԭ��80-160 ����60-180 
#define REC_MAXSIZE          24          //�����Դ�10���ֽ�


#define CMD_START               0xFF
#define CMD_END                 0xB4
typedef struct Single_Bus_Struct{
  bool pre_start;       //���4ms�Ŀ�ʼǰ��־
  bool start_flag;     //���ͽ��еı�־
  uint16_t high_time;   //��¼��ǰ�ĸߵ�ƽʱ��
  uint16_t low_time;    //��¼�͵�ƽʱ��
  uint8_t count;       //��ǰ��bit��
  uint8_t datasum;      //
} bus_struct;

extern bus_struct s_bus;
extern uint8_t rec_data[REC_MAXSIZE];
extern uint8_t test_count;
extern uint16_t record[70];
extern double ADC_data[2];
extern uint16_t tick;
extern bool ext_flag;




/*void Sysclk_config(void);
void Tim2_config(void);
void USART_Config(void);
void ADC_Config(void);
uint16_t User_GetTick(void);*/
void Delay_ms(__IO uint16_t nCount);
void Decrease_Tick(void);

