#include "user.h"
#include "stm8l15x_conf.h"
//#include "math.h"

//uint16_t ADC_Val[2]={0,0};
/*配置外部时钟*/

/*void Sysclk_config(void){
  CLK_HSEConfig(CLK_HSE_ON);
  while(CLK_GetFlagStatus(CLK_FLAG_HSERDY)==RESET);     //等待晶振稳定
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
}

void Tim2_config(void){
  TIM2_DeInit();
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE); //使能外设时钟，STM8L外设时钟默认关闭，使用前需使能
  TIM2_TimeBaseInit(TIM2_Prescaler_128, TIM2_CounterMode_Up, 0xffff);
  TIM2_ARRPreloadConfig(ENABLE); 
 // TIM2_SetAutoreload(0xffff);
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);
  TIM2_Cmd(ENABLE);
}

  
void Delay_halfms(__IO uint16_t nCount){
  uint16_t tickstart = User_GetTick();
  uint16_t wait = nCount;
  if(wait<MAX_DELAY){
    wait +=1;
  }
  while(User_GetTick()-tickstart<wait);
}

uint16_t User_GetTick(void){
  return tick;
}

void Inc_Tick(void){
  tick++;
}

void ADC_Config(void){
  //DMA_DeInit(DMA_Channel0);
  ADC_DeInit(ADC1);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);
  ADC_ChannelCmd(ADC1,ADC_Channel_4,ENABLE);
  ADC_ChannelCmd(ADC1,ADC_Channel_18,ENABLE);
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_9Cycles); 
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
  ADC_Cmd(ADC1,ENABLE);
  ADC_DMACmd(ADC1,ENABLE);
  //DMA_Config
   //DMA_Cmd(DMA_Channel0,ENABLE); 
  //DMA_Init(DMA1);
}

void USART_Config(void){
  USART_DeInit(USART1);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);//开启USART时钟
  GPIO_Init(GPIOC, GPIO_Pin_5,GPIO_Mode_Out_PP_High_Fast);//TXD
  GPIO_Init(GPIOC, GPIO_Pin_6,GPIO_Mode_In_PU_No_IT);//RXD
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Tx|USART_Mode_Rx);//设置USART参数9600，8N1，接收/发送
  //USART_ITConfig (USART1,USART_IT_RXNE,ENABLE);//使能接收中断，中断向量号28
  USART_Cmd (USART1,ENABLE);//使能USART
}*/
