/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "user.h"
#include <string.h>
/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint16_t ADC_Val[2];
extern double ADC_mW;
extern double ADC_mV2;
//bool ext_flag=0;
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

  DMA_ClearITPendingBit(DMA1_IT_TC0);
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  if (PWR_GetFlagStatus(PWR_FLAG_PVDOF) != RESET)//���񵽵�ѹС��3.05V��
  {
    //���ѣ������ź�
    EXTI_DeInit();
    //Bus_SendByte(uint8_t* sendbuf,uint8_t size);  //���͵͵����źţ�֮��ͽ���while����������
  }
  
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  
  EXTI_DeInit();
  
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_FL_No_IT); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
  TIM3_TimeBaseInit(TIM3_Prescaler_16, TIM3_CounterMode_Up, PRESTART_EDGE);       //(4200)us��������ж�
  TIM3_ICInit(TIM3_Channel_1,TIM3_ICPolarity_Rising,TIM3_ICSelection_DirectTI,TIM3_ICPSC_DIV1,0x00);   //���ò���1ͨ�����޷�Ƶ�����˲���
  TIM3_ITConfig(TIM3_IT_Update|TIM3_IT_CC1, ENABLE);                  //����жϺ����벶���ж�
  TIM3_CCxCmd(TIM3_Channel_1,ENABLE);
  TIM3_Cmd(ENABLE);
  //ext_flag |=1;
  s_bus.pre_start |= 1;                         //Ԥ����
  
  
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  tick--;
  TIM2_ClearITPendingBit(TIM2_IT_Update);
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  if(TIM3_GetITStatus(TIM3_IT_Update)!=RESET){
    if(s_bus.pre_start){         //��4ms�ȴ������кʹ�����������������4ms����û��������
      s_bus.pre_start &= 0;         //��־λ���
      s_bus.start_flag &= 0;
      TIM3->CCER1 |= TIM_CCER1_CC1P;      //�޸ĵ�ǰΪ�½���,����4ms�����ͨ�ţ������һ�����ݻ��������������ǲ�������
      //if(ext_flag){s_bus.pre_start|=1;ext_flag=0; TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);}
    }
  
  }
  TIM3_ClearITPendingBit(TIM3_IT_Update);
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  if(TIM3_GetITStatus(TIM3_IT_CC1)!=RESET)
  {
    if(test_count<70) record[test_count++] = TIM3_GetCapture1();  //��¼ǰ30������
    if(TIM3->CCER1>>1 & 1)                                             //�����ǰ���½��ش���(��2λ��1) CC2P:00100000
    {
      
      //��������½�����½����ж�
      if((!s_bus.start_flag)&&(!s_bus.pre_start))       //״̬1�������ǰû�н���4ms�ȴ���û�����ڴ��䣬���½��أ���ô���������͵Ŀ�ʼ
      {
        s_bus.pre_start |= 1;                  //��pre_start��1
        TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);      //���������ش���
      }
      else if(s_bus.pre_start && s_bus.start_flag)        //״̬2����ǰ�ǽ����˴������
      {
        s_bus.high_time=TIM3_GetCapture1();     //ȡ������Ĵ�����ֵ
        if(s_bus.high_time>40 && s_bus.high_time<200)//�жϵ�ǰ�ĸߵ�ƽʱ���Ƿ�����,������������80  160
        {
          TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);    //���������ش���
          //����������
        }
        else
        {
          s_bus.pre_start &=0;       //��־���
          s_bus.start_flag &=0;
          s_bus.high_time = 0;
          //���޸Ĵ������ԣ������½��أ��ȴ�4ms�����ź�
        }
        
        
      }
    }
     else                                                       //��ǰ�������ش���
    {      
      
      s_bus.low_time=TIM3_GetCapture1();                   //��¼��ǰ��ʱ���   
      if(s_bus.pre_start && (!s_bus.start_flag))              //A�Ѿ����½��ص������ǵ�һ�������أ����ǵȴ�4ms��ʱ������һ��������
      {
        if(s_bus.low_time< 3600 || s_bus.low_time>4400)                                            //a �����ǰ�ĵ͵�ƽʱ��С��3800us ������4400������ˣ������ʱ��־λ��ȫ����0
        {
          s_bus.pre_start &=0;
          s_bus.start_flag &=0;
        }
        else                                                              //b ��ǰ�ĵ͵�ƽʱ�����3800��С��4200us
        {
          s_bus.start_flag |= 1;                                             
        }
      }
      else if(s_bus.start_flag && s_bus.pre_start)        //B�жϵ�ǰ״̬  �����ǽ��봫������� 
      {
        
        if(s_bus.high_time<120 && s_bus.high_time>40&&s_bus.low_time<200 && s_bus.low_time>120)           //�жϵ�ƽʱ�� �������80��160 40���������
        {
          data[s_bus.datasum] &= (uint8_t)~(1<<s_bus.count);     //��countλ��Ϊ0
          s_bus.count++;
          if(s_bus.count==8){s_bus.count=0; s_bus.datasum++;}
        }
        else if(s_bus.high_time>120 && s_bus.high_time<200 && s_bus.low_time<120 && s_bus.low_time>40)      //b�ߵ�ƽ��140us���ϣ��͵�ƽ��100us���� ��1
        {
          data[s_bus.datasum] |= (uint8_t)1<<s_bus.count;        //��countλ��1
          s_bus.count++;
          if(s_bus.count==8){s_bus.count=0; s_bus.datasum++;}
        }
        else                                                    //c������� �ָ�Ĭ��״̬
        {
          s_bus.pre_start &= 0;
          s_bus.start_flag &= 0;
        }
      }
    TIM3->CCER1 |= TIM_CCER1_CC1P;     //�޸ĵ�ǰΪ�½���
    }
    TIM3_SetCounter(0x00);                  //����ǰ����ֵ��λ
  }
  TIM3_ClearITPendingBit(TIM3_IT_CC1);
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */		
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   //USART_ClearITPendingBit (USART1,USART_IT_RXNE);//���жϱ�־
   //USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//�����յ������ݷ��ͳ�ȥ
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/