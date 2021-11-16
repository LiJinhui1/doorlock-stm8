#include "stm8l15x_conf.h"
//#include "stdio.h"
#include "user.h"

/*Receive������*/
/*
1���������½��ػ��ѣ�Ȼ��4ms�͵�ƽ������4ms����뷢�����ݣ�����TIM3������жϻ�������һ���ߵ�ƽ�����µ�һ�����ݱ�����
2�����յ���ʼ�źź�ÿ��1s����8�ֽ�����(double����) �ֱ��ǹ��ʺ͵�ص�ѹ,���û�н����źŵĻ������60s��ػ�
3����������3.05V(���3.6Vʱ)���е͵����źš�
��Ҫ�涨��ʼ�źţ������źţ��͵����ź� ����Ϊ0x00

*/

uint16_t ADC_Val[2]={0,0};       //��ӦADCת�����ڴ�
double ADC_data[2]={0};         //Ҫ���͵����ݣ�ADC[0]-PC4��Ӧ��ص�ѹ,ADC[1]-PB0��Ӧ��繦��
uint16_t tick=0;
bus_struct s_bus = {FALSE,FALSE,0,0,0,0};
uint8_t data[MAXSIZE]={0};
uint8_t test_count=0;
uint16_t record[70]={0};
double temp=0;          //double��4���ֽ�
uint8_t patience=0;

/*��ʱ������TIM2�ж�*/
static void Delay_40us(__IO uint16_t nCount){
  tick = nCount;
  while(tick);
}
  //1������Ҫȷ�϶Է��Ƿ����һֱ���գ����Բ�������4ms�� 
  //2��������Ҫ�ڴ������ݵ�ʱ�������ݣ����տ��ܻ����
void Bus_SendByte(uint8_t* sendbuf,uint8_t size){
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_Out_PP_High_Slow);
  GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  for(uint8_t j=0;j!=size;j++)
  {
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
  PWR_FastWakeUpCmd(ENABLE);    //ʹ�ܿ�������
  PWR_UltraLowPowerCmd(ENABLE); //ʹ�ܳ��͹��ģ�����ʱVREF�ر�
  
  //���еĹܽ��õͣ����͹���
  GPIO_ResetBits (GPIOA,GPIO_Pin_All);
  GPIO_ResetBits (GPIOB,GPIO_Pin_All);
  GPIO_ResetBits (GPIOC,GPIO_Pin_All);
  GPIO_ResetBits (GPIOD,GPIO_Pin_All);
  
  /*�ⲿ�жϳ�ʼ��*/
  EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_IT);
  
  
  
  /*ADC_Config����������*/
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  GPIO_Init(ADC_1PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(ADC_2PORT,ADC_2PINS,GPIO_Mode_In_FL_No_IT);
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_384Cycles);  //0-23ͨ��������ͨ��
  ADC_ChannelCmd(ADC1,ADC_Channel_4,ENABLE);
  ADC_ChannelCmd(ADC1,ADC_Channel_18,ENABLE);
  
  
  //��ʼ��TIM2����ֹ���Ѻ�û���ݣ���20sʱ�䣬TIM2��update��40us
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE); //ʹ������ʱ�ӣ�STM8L����ʱ��Ĭ�Ϲرգ�ʹ��ǰ��ʹ��
  TIM2_TimeBaseInit(TIM2_Prescaler_16,TIM2_CounterMode_Up,0x27);  //16M/16/(39+1) us ÿ40us����һ���ж� ����ʱ��ҪTIM2
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);
  TIM2_Cmd(ENABLE);  
  
  /*DMA1_Config����������*/
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0); //Connect ADC to DMA channel 0 
  DMA_Init(DMA1_Channel0,(uint32_t)&ADC_Val,0x5344,2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular,DMA_MemoryIncMode_Inc,DMA_Priority_VeryHigh,DMA_MemoryDataSize_HalfWord); //0x5344�Ǹ�8λADC�ĵ�ַ
  DMA_Cmd(DMA1_Channel0,ENABLE);
  DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);
  
  
  /*Other GPIO CONFIG*/
  GPIO_Init(LED_PORT,LED_PINS,GPIO_Mode_Out_PP_High_Slow);//��ʼ��LED�˿�
  
  
  enableInterrupts();

  
  
  /*RX*/
  while (1)
  {
    //���Ѻ�ִ��
    //��������Ҫ������60s�ڣ��������60s�ˣ��ͽ�������
    if(data[0]==CMD_START)
    { 
      //ADC��DMA��ʼ����
      //����Dset������ģʽ��3us�Ľ���ʱ��
      while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF));//�����ǿ��ٻ��ѣ�����Ҫȷ��Vrefintf��������
      ADC_Cmd(ADC1,ENABLE);
      DMA_GlobalCmd(ENABLE);
      ADC_SoftwareStartConv(ADC1);
      temp =  (double)ADC_Val[0]*3.3/4.096;
      ADC_data[1] = (double)(-5.44e-08 *pow(temp,3)+1.55e-04 *pow(temp,2)-6.04e-03 *temp+0.6688);
      ADC_data[0] = (double)ADC_Val[1]*3.3/4.096;
      //�ر�Dset,��ʼ���

  
      //Bus_SendByte((uint8_t*) ADC_data,8);      //����1��double������4�ֽڵ� 2��double��8�ֽ�,�ȵ����͵�ʱ���ٲ���
      Delay_40us(25000);     //25*40Ϊ1ms,25000*40��1s
      patience++; 
    }
    
    /*else if(data[0]==CMD_END)        //����Ӧ����ֹͣ���ͣ��������ߣ����ú��ⲿ�ж�
    {
      //Ϊ��������׼����ֹͣһЩ����Ĺ���
      
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
    {               //��С�ı����ѣ�û���ź�
      Delay_40us(25000); //�ȴ�1s
      patience++;
      if(patience==60)  //60s���޿�ʼ�źţ�����
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
