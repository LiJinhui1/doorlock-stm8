#include "stm8l15x_conf.h"
//#include "stdio.h"
#include "user.h"

/*Receive的例程*/
/*
1、唤醒是下降沿唤醒，然后4ms低电平，唤醒4ms后必须发送数据，否则TIM3的溢出中断会跳过第一个高电平，导致第一个数据被忽略
2、接收到开始信号后，每隔1s发送8字节数据(double类型) 分别是功率和电池电压,如果没有结束信号的话，坚持60s后关机
3、电量低于3.05V(电池3.6V时)上行低电量信号。
需要规定开始信号，结束信号，低电量信号 不可为0x00

*/

uint16_t ADC_Val[2]={0,0};       //对应ADC转换的内存
double ADC_data[2]={0};         //要发送的数据，ADC[0]-PC4对应电池电压,ADC[1]-PB0对应充电功率
uint16_t tick=0;
bus_struct s_bus = {FALSE,FALSE,0,0,0,0};
uint8_t data[MAXSIZE]={0};
uint8_t test_count=0;
uint16_t record[70]={0};
double temp=0;          //double是4个字节
uint8_t patience=0;

/*延时函数，TIM2中断*/
static void Delay_40us(__IO uint16_t nCount){
  tick = nCount;
  while(tick);
}
  //1、这里要确认对方是否可以一直接收，可以不用拉低4ms吗 
  //2、尽量不要在处理数据的时候传来数据，接收可能会出错
void Bus_SendByte(uint8_t* sendbuf,uint8_t size){
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_Out_PP_High_Slow);
  GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  for(uint8_t j=0;j!=size;j++)
  {
      Delay_40us(100);      //拉低4ms
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
  }

  GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS, GPIO_Mode_In_PU_No_IT);
}

static double pow(double val,uint8_t n){
  double temp=1;
  while(n--){
    temp*=val;
  }
  return temp;
}


void main(void)
{
  /*sysclock_config*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  
  //PVD_config
  PWR_PVDLevelConfig(PWR_PVDLevel_3V05);
  PWR_PVDCmd(ENABLE);
  PWR_PVDITConfig(ENABLE);
  PWR_FastWakeUpCmd(ENABLE);    //使能快速启动
  PWR_UltraLowPowerCmd(ENABLE); //使能超低功耗，休眠时VREF关闭
  
  //所有的管脚置低，降低功耗
  GPIO_ResetBits (GPIOA,GPIO_Pin_All);
  GPIO_ResetBits (GPIOB,GPIO_Pin_All);
  GPIO_ResetBits (GPIOC,GPIO_Pin_All);
  GPIO_ResetBits (GPIOD,GPIO_Pin_All);
  
  /*外部中断初始化*/
  EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_IT);
  
  
  
  /*ADC_Config，但不启动*/
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  GPIO_Init(ADC_1PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(ADC_2PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_384Cycles);  //0-23通道是慢速通道
  ADC_ChannelCmd(ADC1,ADC_Channel_4,ENABLE);
  ADC_ChannelCmd(ADC1,ADC_Channel_18,ENABLE);
  
  
  //初始化TIM2，防止唤醒后没数据，计20s时间，TIM2的update在40us
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE); //使能外设时钟，STM8L外设时钟默认关闭，使用前需使能
  TIM2_TimeBaseInit(TIM2_Prescaler_16,TIM2_CounterMode_Up,0x27);  //16M/16/(39+1) us 每40us进入一次中断 发送时需要TIM2
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);
  TIM2_Cmd(ENABLE);  
  
  /*DMA1_Config，但不启动*/
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0); //Connect ADC to DMA channel 0 
  DMA_Init(DMA1_Channel0,(uint32_t)&ADC_Val,0x5344,2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular,DMA_MemoryIncMode_Inc,DMA_Priority_VeryHigh,DMA_MemoryDataSize_HalfWord); //0x5344是高8位ADC的地址
  DMA_Cmd(DMA1_Channel0,ENABLE);
  DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);
  
  
  /*Other GPIO CONFIG*/
  GPIO_Init(LED_PORT,LED_PINS,GPIO_Mode_Out_PP_High_Slow);//初始化LED端口
  
  
  enableInterrupts();

  
  
  /*RX*/
  while (1)
  {
    //唤醒后执行
    //所有下面要控制在60s内，如果到达60s了，就进入休眠
    if(data[0]==CMD_START)
    { 
      //ADC与DMA开始启动
      //开启Dset，测量模式有3us的建立时间
      while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF));//由于是快速唤醒，这里要确保Vrefintf建立好了
      ADC_Cmd(ADC1,ENABLE);
      DMA_GlobalCmd(ENABLE);
      ADC_SoftwareStartConv(ADC1);
      temp =  (double)ADC_Val[0]*3.3/4.096;
      ADC_data[1] = (double)(-5.44e-08 *pow(temp,3)+1.55e-04 *pow(temp,2)-6.04e-03 *temp+0.6688);
      ADC_data[0] = (double)ADC_Val[1]*3.3/4.096;
      //关闭Dset,开始充电

  
      //Bus_SendByte((uint8_t*) ADC_data,8);      //由于1个double变量是4字节的 2个double是8字节,等到发送的时候再测试
      Delay_40us(25000);     //25*40为1ms,25000*40是1s
      patience++; 
    }
    
    /*else if(data[0]==CMD_END)        //这里应该是停止发送，进入休眠，设置好外部中断
    {
      //为唤醒做好准备，停止一些外设的工作
      
      patience=0;
      TIM3_DeInit();
      ADC_Cmd(ADC1,DISABLE);
      DMA_GlobalCmd(DISABLE);
      EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);
      GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_IT);
      data[0]=0;
      halt();
    }
    
    else
    {               //不小心被唤醒，没有信号
      Delay_40us(25000); //等待1s
      patience++;
      if(patience==60)  //60s后无开始信号，休眠
      {
        patience=0;
        TIM3_DeInit();
        ADC_Cmd(ADC1,DISABLE);
        DMA_GlobalCmd(DISABLE);
        EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);
        GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_IT);
        data[0]=0;
        halt();
      }
    }*/

    
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
