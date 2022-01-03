#include "stm8l15x_conf.h"
//#include "stdio.h"
#include "user.h"
/*Send!!*/



/*延时函数，TIM3中断*/
void Delay_40us(__IO uint16_t nCount){
  tick = nCount;
  while(tick);
}
static void TIM2_Config(void){
  //初始化TIM2，防止唤醒后没数据，计20s时间，TIM2的update在40us
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE); //使能外设时钟，STM8L外设时钟默认关闭，使用前需使能
  TIM2_TimeBaseInit(TIM2_Prescaler_16,TIM2_CounterMode_Up,0x27);  //16M/16/(39+1) us 每40us进入一次中断 发送时需要TIM2
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);
  TIM2_Cmd(ENABLE);  
}

void TIM3_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
  TIM3_TimeBaseInit(TIM3_Prescaler_16, TIM3_CounterMode_Up, PRESTART_EDGE);       //(4200)us进入溢出中断
  if(ext_flag)
  TIM3_ICInit(TIM3_Channel_1,TIM3_ICPolarity_Rising,TIM3_ICSelection_DirectTI,TIM3_ICPSC_DIV1,0x00);   //配置捕获1通道，无分频，无滤波器
  else TIM3_ICInit(TIM3_Channel_1,TIM3_ICPolarity_Falling,TIM3_ICSelection_DirectTI,TIM3_ICPSC_DIV1,0x00);
  TIM3_ITConfig(TIM3_IT_Update|TIM3_IT_CC1, ENABLE);                  //溢出中断和输入捕获中断
  TIM3_CCxCmd(TIM3_Channel_1,ENABLE);
  TIM3_Cmd(ENABLE);
}
/*发送数据的函数 先发低位再发高位，先发高地址再发低地址*/
void Bus_SendByte(uint8_t* sendbuf,uint8_t size){
  TIM3_DeInit(); //由于发送数据的时候这个管脚有电平变化，所以不能进入捕获中断，禁止TIM3
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_Out_PP_High_Slow);
  GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  Delay_40us(100);      //放在这里只会拉低一次4ms，后续可接连传输，用于给CC2530传数据
  for(uint8_t j=0;j!=size;j++)
  {
    //GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
    //Delay_40us(100);      //拉低4ms,放在这里每8位都会拉低4ms，给STM8传数据
    for(uint8_t i=0; i!=8;++i)
    {
      if((*sendbuf>>i) & 1)       //如果当前位是1
      {
        GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
        Delay_40us(4);     //40*n=160us
        GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
        Delay_40us(2);   
      }
      else
      {
        GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
        Delay_40us(2);
        GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
        Delay_40us(4);
      }
    }
    sendbuf++;
    GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
    //Delay_40us(6);              //每个字节发送后要拉高240us，表示单字节发送结束
  }
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS, GPIO_Mode_In_PU_No_IT);
  GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  TIM3_Config();
}

void main(void)
{
  /*sysclock_config*/
  CLK_HSEConfig(CLK_HSE_ON);
  while(CLK_GetFlagStatus(CLK_FLAG_HSERDY)==RESET);     //等待晶振稳定
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  
  TIM2_Config();
  /*TIM3_config TIM3:send data timer delay interrupt*/
  TIM3_Config();
  
  
  /*设置发送引脚*/
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_No_IT);       //SingleBusGPIO 上拉输入，中断
  GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  
  
  /*Other GPIO CONFIG*/
  GPIO_Init(LED_PORT,LED_PINS,GPIO_Mode_Out_PP_High_Slow);//初始化LED端口
  
  enableInterrupts();

  uint8_t start =  0xB3;
  uint8_t other = 0xCC;
  uint8_t end = 0xB4;
  //uint8_t act_doorlock[4]={0xC0,0x00,0x00,0x3F};
  bool flag =1;
  /*TXCode*/
  while (1)
  {
    if(CLK_GetSYSCLKSource()== CLK_SYSCLKSource_HSE){              //如果时钟切换成功
      if(flag)
      {  
         /*LED_ON;
         Bus_SendByte(&start,1);
         Delay_40us(25000);
         LED_OFF;
         Delay_40us(25000);
         Bus_SendByte(&other,1);
         Delay_40us(12500);
         LED_ON;
         Bus_SendByte(&end,1);
         flag&=0;*/
        LED_ON;
        Bus_SendByte(&start,1);
        flag&=0;
      }
      
    }

  }
}






#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
