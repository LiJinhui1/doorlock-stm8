#include "stm8l15x.h"
//定义LED及按键端口
#define LED_PORT        GPIOD
#define LED_PINS        GPIO_Pin_0
#define KEY_PORT        GPIOB
#define KEY_PINS        GPIO_Pin_1
#define USART_TX_PORT   GPIOC
#define USART_TX_PINS   GPIO_Pin_5
#define USART_RX_PORT   GPIOC
#define USART_RX_PINS   GPIO_Pin_6
#define ADC_1PORT       GPIOC
#define ADC_1PINS       GPIO_Pin_4
#define ADC_2PORT       GPIOB
#define ADC_2PINS       GPIO_Pin_0
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define LED_TOGGLE      GPIO_ToggleBits(LED_PORT,LED_PINS)
#define LED_OFF         GPIO_SetBits(LED_PORT,LED_PINS)
#define LED_ON          GPIO_ResetBits(LED_PORT,LED_PINS)
#define MAX_DELAY       0xFFFFU
#define SINGLEBUS_PORT       GPIOB
#define SINGLEBUS_PINS       GPIO_Pin_1
#define PRESTART_EDGE        0x112F     //溢出中断在(4399+1)us
#define RIGHT_EDGE           0xB3       // 最大容错在180us内(179+1)
#define LEFT_EDGE            0x3B         //最小的边界在60us(59+1) 原来80-160 现在60-180 


#define MAXSIZE              10           //最大可以存10个字节
#define CMD_START            0xB3
#define CMD_END              0x00
typedef struct Single_Bus_Struct{
  bool pre_start;       //标记4ms的开始前标志
  bool start_flag;     //传送进行的标志
  uint16_t high_time;   //记录当前的高电平时间
  uint16_t low_time;    //记录低电平时间
  uint8_t count;       //当前的bit数
  uint8_t datasum;      //
} bus_struct;

extern uint16_t tick;
extern bus_struct s_bus;
extern uint8_t data[MAXSIZE];
extern uint8_t test_count;
extern uint16_t record[70];
extern double ADC_data[2];

void Bus_SendByte(uint8_t* sendbuf,uint8_t size);




