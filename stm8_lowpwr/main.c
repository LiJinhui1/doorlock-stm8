#include "stm8l15x_conf.h"
#include "user.h"

/*V1.0*/
/*
1���½��ػ��ѣ�����4ms�����ֽڷ���ʱÿ�ֽڼ���ߵ�ƽ240us��
2����ʼ�źţ�0xB3,�����ź�0xB4,
3��ʵ�ʽ��յ��ĵ�ƽ������ƽ120-150        �̵�ƽ��׽40-70
4������ʱ�䣺2021/11/20 �����ݷ��ͣ������ݽ��գ������ݷ�������գ�ADCת�����ͣ��ظ������봫��ADCû���⣬
5�����Ѿ�����͵�ʱ��������1s����һ�Σ����Է��ͽ�����־��ʱ������n.5s���ҷ���
6�����绽�ѱ����ultralow power�رգ���Ϊ�Ƚϵ�ѹ��ҪVRef
7���й���RTC�����ã�����رյ�Դ�����ܸ������á���ʹ��λ��֮�󣨰���������λ����¼��λ����Ч��
*/

uint16_t ADC_Val[2]={0,0};       //���ADCת��������
double ADC_data[2]={0};         //Ҫ���͵����ݣ�ADC[0]-PC4��Ӧ��ص�ѹ,ADC[1]-PB0��Ӧ��繦��
bus_struct s_bus = {FALSE,FALSE,0,0,0,0};
uint8_t rec_data[REC_MAXSIZE]={0};
uint8_t test_count=0;
uint16_t record[70]={0};
double temp=0;          //double��4���ֽ�
uint8_t lpwr = CMD_LWPWR_FLAG;

/*��ʱ������TIM2�ж�*/
static void Delay_40us(__IO uint16_t nCount){
  tick = nCount;
  while(tick);
}

//TIM3���ò��ҿ�������������
void TIM3_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
  TIM3_TimeBaseInit(TIM3_Prescaler_16, TIM3_CounterMode_Up, PRESTART_EDGE);       //(4200)us��������ж�
  if(ext_flag)
  TIM3_ICInit(TIM3_Channel_1,TIM3_ICPolarity_Rising,TIM3_ICSelection_DirectTI,TIM3_ICPSC_DIV1,0x00);   //���ò���1ͨ�����޷�Ƶ�����˲���
  else TIM3_ICInit(TIM3_Channel_1,TIM3_ICPolarity_Falling,TIM3_ICSelection_DirectTI,TIM3_ICPSC_DIV1,0x00);
  TIM3_ITConfig(TIM3_IT_Update|TIM3_IT_CC1, ENABLE);                  //����жϺ����벶���ж�
  TIM3_CCxCmd(TIM3_Channel_1,ENABLE);
  TIM3_Cmd(ENABLE);
}

/*���绽��Config*/
static void PVD_Config(void){
    //PVD_config
  PWR_PVDLevelConfig(PWR_PVDLevel_3V05);
  PWR_PVDCmd(ENABLE);
  PWR_PVDITConfig(ENABLE);
  PWR_FastWakeUpCmd(ENABLE);    //ʹ�ܿ�������
  //PWR_UltraLowPowerCmd(ENABLE); //ʹ�ܳ��͹��ģ�����ʱVREF�ر�,���ܿ��� ����Ļ�������PVD
  PWR_UltraLowPowerCmd(DISABLE);
}

 /*ADC_Config����������*/
static void ADC_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  GPIO_Init(ADC_1PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(ADC_2PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_384Cycles);  //0-23ͨ��������ͨ�� ����ʱ��384/8M=48us
  
  ADC_ChannelCmd(ADC1,ADC_Channel_4,ENABLE);
  ADC_ChannelCmd(ADC1,ADC_Channel_18,ENABLE);
}

/*DMA1_Config����������*/
static void DMA_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0); //Connect ADC to DMA channel 0 
  DMA_Init(DMA1_Channel0,(uint32_t)&ADC_Val,0x5344,2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular,DMA_MemoryIncMode_Inc,DMA_Priority_VeryHigh,DMA_MemoryDataSize_HalfWord); //0x5344�Ǹ�8λADC�ĵ�ַ
  DMA_Cmd(DMA1_Channel0,ENABLE);
}

/*TIM2���á�����*/
static void TIM2_Config(void){
  //��ʼ��TIM2����ֹ���Ѻ�û���ݣ���20sʱ�䣬TIM2��update��40us
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE); //ʹ������ʱ�ӣ�STM8L����ʱ��Ĭ�Ϲرգ�ʹ��ǰ��ʹ��
  TIM2_TimeBaseInit(TIM2_Prescaler_16,TIM2_CounterMode_Up,0x27);  //16M/16/(39+1) us ÿ40us����һ���ж� ����ʱ��ҪTIM2
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);
  TIM2_Cmd(ENABLE);  
}

/*RTC_Config��һ��������Ҫ����*/
static void RTC_Config(void){
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC,ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI,CLK_RTCCLKDiv_1);
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits );  //��������:32.768/38��ʵ������0.896s
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_SetWakeUpCounter(4012);      //0.896s ����һ�� 1hԼ��4012+1�� ���ڲ���1+1
  RTC_WakeUpCmd(ENABLE);        //����3600/0.897�õ�Լ4013��4012+1��������װֵ4012
}
/*���ݷ��ͺ���,��������ʱ��TIM3�ж�ȡ�����ܽ�����*/
void Bus_SendByte(uint8_t* sendbuf,uint8_t size){
  TIM3_DeInit(); //���ڷ������ݵ�ʱ������ܽ��е�ƽ�仯�����Բ��ܽ��벶���жϣ���ֹTIM3
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_Out_PP_High_Slow);
  for(uint8_t j=0;j!=size;j++)
  {
    GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
    Delay_40us(100);      //����4ms
    for(uint8_t i=0; i!=8;++i)
    {
      if((*sendbuf>>i) & 1)       //�����ǰλ��1
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
    Delay_40us(6);              //ÿ���ֽڷ��ͺ�Ҫ����240us,��ʾ���ֽڷ��ͽ���
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
  CLK_LSICmd(ENABLE);   //LSI������������ر�
  CLK_HaltConfig(CLK_Halt_FastWakeup,DISABLE);
  //���еĹܽ��õͣ����͹���
  GPIO_ResetBits (GPIOA,GPIO_Pin_All);
  GPIO_ResetBits (GPIOB,GPIO_Pin_All);
  GPIO_ResetBits (GPIOC,GPIO_Pin_All);
  GPIO_ResetBits (GPIOD,GPIO_Pin_All);
  /*�ⲿ�жϳ�ʼ��*/
  EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_IT);
  PVD_Config();
  ADC_Config();
  DMA_Config();
  TIM2_Config();        //TIM2�������Ҫ����
  RTC_Config();
  /*Other GPIO CONFIG*/
  GPIO_Init(LED_PORT,LED_PINS,GPIO_Mode_Out_PP_High_Slow);//��ʼ��LED�˿�
  GPIO_Init(DSET_PORT,DSET_PINS,GPIO_Mode_Out_PP_Low_Slow);//���ÿ�������Ϊ�͵�ƽ�����
  GPIO_Init(LOCATE_CTR_PORT,LOCATE_CTR_PINS,GPIO_Mode_Out_PP_Low_Slow); //������Χ��·��ƣ�Ŀǰ�ǵ͵�ƽ
  enableInterrupts();
  while (1)
  {
    Delay_40us(25000);
    if(PVD_flag)
    {
      LED_ON;
      Bus_SendByte(&lpwr,1);    //���͵͵�������
      Delay_40us(25000);
      PVD_flag &= 0;
    }
    
    if(RTC_AWK_flag)          //��1h����ʱ�����������͹��ʣ��ϴ�
    {
      LED_ON;
      MEASURE_ON;
      while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF)!=SET);//�����ǿ��ٻ��ѣ�����Ҫȷ��Vrefintf��������
      ADC_Cmd(ADC1,ENABLE);
      DMA_GlobalCmd(ENABLE);
      ADC_SoftwareStartConv(ADC1);
      Delay_40us(25000);      //��ʱ1s����������1s�ڽ���ֹͣ�ź�
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
      while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF)!=SET);//�����ǿ��ٻ��ѣ�����Ҫȷ��Vrefintf��������
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
        Delay_40us(25000);      //��ʱ1s����������1s�ڽ���ֹͣ�ź�
      }
      MEASURE_OFF;      //�յ������ź�
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
