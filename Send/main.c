#include "stm8l15x_conf.h"
//#include "stdio.h"
#include "user.h"
/*Send!!*/



/*��ʱ������TIM3�ж�*/
void Delay_40us(__IO uint16_t nCount){
  tick = nCount;
  while(tick);
}

/*�������ݵĺ��� �ȷ���λ�ٷ���λ���ȷ��ߵ�ַ�ٷ��͵�ַ*/
void Bus_SendData(uint8_t* sendbuf){
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_Out_PP_High_Slow);
  GPIO_ResetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);        //����4ms
  Delay_40us(100);
  for(uint8_t i=0; i!=8;++i){
    if((*sendbuf>>i) & 1)       //�����ǰλ��1
    {
      GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
      Delay_40us(4);     //40*n
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
  GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
}

void main(void)
{
  /*sysclock_config*/
  CLK_HSEConfig(CLK_HSE_ON);
  while(CLK_GetFlagStatus(CLK_FLAG_HSERDY)==RESET);     //�ȴ������ȶ�
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  
  
  /*TIM3_config TIM3:send data timer delay interrupt*/
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE); //ʹ������ʱ�ӣ�STM8L����ʱ��Ĭ�Ϲرգ�ʹ��ǰ��ʹ��
  TIM3_TimeBaseInit(TIM3_Prescaler_16,TIM3_CounterMode_Up,0x27);  //16M/16/(39+1) us ÿ40us����һ���ж� ,�����ź���Ҫ�������
  TIM3_ITConfig(TIM3_IT_Update , ENABLE);
  TIM3_Cmd(ENABLE);
  
  
  /*���÷�������*/
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_PU_No_IT);       //SingleBusGPIO �������룬�ж�
  GPIO_SetBits(SINGLEBUS_PORT,SINGLEBUS_PINS);
  
  
  /*Other GPIO CONFIG*/
  GPIO_Init(LED_PORT,LED_PINS,GPIO_Mode_Out_PP_High_Slow);//��ʼ��LED�˿�
  
  enableInterrupts();

  uint8_t start =  0xB3;
  uint8_t other = 0xCC;
  bool flag =1;
  /*TXCode*/
  while (1)
  {
    if(CLK_GetSYSCLKSource()== CLK_SYSCLKSource_HSE){              //���ʱ���л��ɹ�
      if(flag)
      {   
          //Delay_40us(50000);      
          //Delay_40us(25000);
          Delay_40us(25000);
          Delay_40us(50000);
          Delay_40us(50000);
        
        
          Delay_40us(50000);      //��ʱ7s����һ������
          Bus_SendData(&start);
          //Bus_SendData(&other);
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
