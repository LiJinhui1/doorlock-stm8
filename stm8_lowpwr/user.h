#include "stm8l15x.h"
//����LED�������˿�
#define LED_PORT        GPIOD
#define LED_PINS        GPIO_Pin_0
#define USART_TX_PORT   GPIOC
#define USART_TX_PINS   GPIO_Pin_5
#define USART_RX_PORT   GPIOC
#define USART_RX_PINS   GPIO_Pin_6
#define ADC_1PORT       GPIOC
#define ADC_1PINS       GPIO_Pin_4
#define ADC_2PORT       GPIOB
#define ADC_2PINS       GPIO_Pin_0
#define DSET_PORT       GPIOB
#define DSET_PINS       GPIO_Pin_3
#define LOCATE_CTR_PORT GPIOB
#define LOCATE_CTR_PINS GPIO_Pin_4
#define BAT_MEASURE_CTL_PORT    GPIOC
#define BAT_MEASURE_CTL_PINS    GPIO_Pin_0
#define BAT_MEASURE_ON      GPIO_SetBits(DSET_PORT,DSET_PINS)
#define BAT_MEASURE_OFF     GPIO_ResetBits(DSET_PORT,DSET_PINS)
#define MEASURE_ON      GPIO_SetBits(DSET_PORT,DSET_PINS)
#define MEASURE_OFF     GPIO_ResetBits(DSET_PORT,DSET_PINS)
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define LED_TOGGLE      GPIO_ToggleBits(LED_PORT,LED_PINS)
#define LED_OFF         GPIO_ResetBits(LED_PORT,LED_PINS)
#define LED_ON          GPIO_SetBits(LED_PORT,LED_PINS)
#define MAX_DELAY       0xFFFFU
#define SINGLEBUS_PORT       GPIOB
#define SINGLEBUS_PINS       GPIO_Pin_1
#define USER_KEY_PORT        GPIOB
#define USER_KEY_PINS        GPIO_Pin_6   
#define PRESTART_EDGE        0x112F     //����ж���(4399+1)us


#define REC_MAXSIZE          30           //�����Դ�10���ֽ�
#define CMD_START            0xB3
#define CMD_END              0xB4
#define CMD_LWPWR_FLAG       0xB5
typedef struct Single_Bus_Struct{
  bool pre_start;       //���4ms�Ŀ�ʼǰ��־
  bool start_flag;     //���ͽ��еı�־
  uint16_t high_time;   //��¼��ǰ�ĸߵ�ƽʱ��
  uint16_t low_time;    //��¼�͵�ƽʱ��
  uint8_t count;       //��ǰ��bit��
  uint8_t datasum;      //
} bus_struct;

extern uint16_t tick;
extern bus_struct s_bus;
extern uint8_t rec_data[REC_MAXSIZE];
extern uint8_t test_count;
extern uint16_t record[70];
extern double ADC_data[2];
extern bool RTC_AWK_flag;
extern bool PVD_flag;
extern bool ext_flag;

void Bus_SendByte(uint8_t* sendbuf,uint8_t size);
void TIM3_Config(void);



