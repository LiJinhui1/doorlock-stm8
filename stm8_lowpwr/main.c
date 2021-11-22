#include "stm8l15x_conf.h"
#include "user.h"

/*V1.0*/
/*
1、下降沿唤醒，拉低4ms，多字节发送时每字节间隔高电平240us，
2、开始信号：0xB3,结束信号0xB4,
3、实际接收到的电平：长电平120-150        短电平捕捉40-70
4、测试时间：2021/11/20 多数据发送，多数据接收，单数据发送与接收，ADC转换传送，重复唤醒与传输ADC没问题，
5、当已经激活发送的时候，由于是1s发送一次，所以发送结束标志的时候尽量在n.5s左右发送
6、掉电唤醒必须把ultralow power关闭，因为比较电压需要VRef
7、有关于RTC的设置，必须关闭电源，才能更新配置。即使复位了之后（包括按键复位，烧录复位都无效）
*/

uint16_t ADC_Val[2]={0,0};       //存放ADC转换的数据
double ADC_data[2]={0};         //要发送的数据，ADC[0]-PC4对应电池电压,ADC[1]-PB0对应充电功率
bus_struct s_bus = {FALSE,FALSE,0,0,0,0};
uint8_t rec_data[REC_MAXSIZE]={0};
uint8_t test_count=0;
uint16_t record[70]={0};
double temp=0;          //double是4个字节
uint8_t lpwr = CMD_LWPWR_FLAG;

/*延时函数，TIM2中断*/
static void Delay_40us(__IO uint16_t nCount){
  tick = nCount;
  while(tick);
}

//TIM3配置并且开启，接收数据
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

/*掉电唤醒Config*/
static void PVD_Config(void){
    //PVD_config
  PWR_PVDLevelConfig(PWR_PVDLevel_3V05);
  PWR_PVDCmd(ENABLE);
  PWR_PVDITConfig(ENABLE);
  PWR_FastWakeUpCmd(ENABLE);    //使能快速启动
  //PWR_UltraLowPowerCmd(ENABLE); //使能超低功耗，休眠时VREF关闭,不能开启 否则的话读不出PVD
  PWR_UltraLowPowerCmd(DISABLE);
}

 /*ADC_Config，但不启动*/
static void ADC_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  GPIO_Init(ADC_1PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(ADC_2PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_384Cycles);  //0-23通道是慢速通道 采样时间384/8M=48us
  
  ADC_ChannelCmd(ADC1,ADC_Channel_4,ENABLE);
  ADC_ChannelCmd(ADC1,ADC_Channel_18,ENABLE);
}

/*DMA1_Config，但不启动*/
static void DMA_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0); //Connect ADC to DMA channel 0 
  DMA_Init(DMA1_Channel0,(uint32_t)&ADC_Val,0x5344,2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular,DMA_MemoryIncMode_Inc,DMA_Priority_VeryHigh,DMA_MemoryDataSize_HalfWord); //0x5344是高8位ADC的地址
  DMA_Cmd(DMA1_Channel0,ENABLE);
}

/*TIM2配置、启动*/
static void TIM2_Config(void){
  //初始化TIM2，防止唤醒后没数据，计20s时间，TIM2的update在40us
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE); //使能外设时钟，STM8L外设时钟默认关闭，使用前需使能
  TIM2_TimeBaseInit(TIM2_Prescaler_16,TIM2_CounterMode_Up,0x27);  //16M/16/(39+1) us 每40us进入一次中断 发送时需要TIM2
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);
  TIM2_Cmd(ENABLE);  
}

/*RTC_Config，一旦开启就要启动*/
static void RTC_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC,ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI,CLK_RTCCLKDiv_1);
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits );  //理论周期:32.768/38，实际周期0.896s
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_SetWakeUpCounter(4012);      //0.896s 唤醒一次 1h约（4012+1） 现在测试1+1
  RTC_WakeUpCmd(ENABLE);        //按照3600/0.897得到约4013（4012+1）设置重装值4012
}
/*数据发送函数,包括接收时的TIM3中断取消，管脚配置*/
void Bus_SendByte(uint8_t* sendbuf,uint8_t size){
  TIM3_DeInit(); //由于发送数据的时候这个管脚有电平变化，所以不能进入捕获中断，禁止TIM3
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_Out_PP_High_Slow);
  for(uint8_t j=0;j!=size;j++)
  {
    GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
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
    sendbuf++;
    GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
    Delay_40us(6);              //每个字节发送后要拉高240us,表示单字节发送结束
  }
  GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS, GPIO_Mode_In_PU_No_IT);
  TIM3_Config();
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
  CLK_LSICmd(ENABLE);   //LSI开启后不能随意关闭
  CLK_HaltConfig(CLK_Halt_FastWakeup,DISABLE);
  //所有的管脚置低，降低功耗
  GPIO_ResetBits (GPIOA,GPIO_Pin_All);
  GPIO_ResetBits (GPIOB,GPIO_Pin_All);
  GPIO_ResetBits (GPIOC,GPIO_Pin_All);
  GPIO_ResetBits (GPIOD,GPIO_Pin_All);
  /*外部中断初始化*/
  EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_IT);
  PVD_Config();
  ADC_Config();
  DMA_Config();
  TIM2_Config();        //TIM2醒来后就要启动
  RTC_Config();
  /*Other GPIO CONFIG*/
  GPIO_Init(LED_PORT,LED_PINS,GPIO_Mode_Out_PP_High_Slow);//初始化LED端口
  GPIO_Init(DSET_PORT,DSET_PINS,GPIO_Mode_Out_PP_Low_Slow);//设置控制引脚为低电平，充电
  GPIO_Init(LOCATE_CTR_PORT,LOCATE_CTR_PINS,GPIO_Mode_Out_PP_Low_Slow); //根据外围电路设计，目前是低电平
  enableInterrupts();
  while (1)
  {
    Delay_40us(25000);
    if(PVD_flag)
    {
      LED_ON;
      Bus_SendByte(&lpwr,1);    //发送低电量提醒
      Delay_40us(25000);
      PVD_flag &= 0;
    }
    
    if(RTC_AWK_flag)          //当1h唤醒时，测量电量和功率，上传
    {
      LED_ON;
      MEASURE_ON;
      while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF)!=SET);//由于是快速唤醒，这里要确保Vrefintf建立好了
      ADC_Cmd(ADC1,ENABLE);
      DMA_GlobalCmd(ENABLE);
      ADC_SoftwareStartConv(ADC1);
      Delay_40us(25000);      //延时1s，可以在这1s内接收停止信号
      temp =  (double)ADC_Val[0]*3.3/4.096;
      ADC_data[1] = (double)(-5.44e-08 *pow(temp,3)+1.55e-04 *pow(temp,2)-6.04e-03 *temp+0.6688);
      ADC_data[0] = (double)ADC_Val[1]*3.3/4.096;
      MEASURE_OFF;
      Bus_SendByte((uint8_t*)ADC_data,8);
      RTC_AWK_flag &= 0;
    }
    
    if(rec_data[0]==CMD_START)
    {
      LED_ON;
      MEASURE_ON;
      while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF)!=SET);//由于是快速唤醒，这里要确保Vrefintf建立好了
      ADC_Cmd(ADC1,ENABLE);
      DMA_GlobalCmd(ENABLE);
      ADC_SoftwareStartConv(ADC1);
      while(rec_data[1]!=CMD_END)
      {
        temp =  (double)ADC_Val[0]*3.3/4.096;
        ADC_data[1] = (double)(-5.44e-08 *pow(temp,3)+1.55e-04 *pow(temp,2)-6.04e-03 *temp+0.6688);
        ADC_data[0] = (double)ADC_Val[1]*3.3/4.096;
        MEASURE_OFF;
        Bus_SendByte((uint8_t*)ADC_data,8);
        Delay_40us(25000);      //延时1s，可以在这1s内接收停止信号
      }
      MEASURE_OFF;      //收到结束信号
    }
    TIM3_DeInit();
    ADC_Cmd(ADC1,DISABLE);
    DMA_GlobalCmd(DISABLE);
    EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);
    GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_IT);
    rec_data[0]=0;
    rec_data[1]=0;
    s_bus.datasum=0;
    s_bus.count=0;
    LED_OFF;
    halt();
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
