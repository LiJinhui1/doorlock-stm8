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
uint16_t tick=0;
bool ext_flag=0;
bool RTC_AWK_flag=0;
bool PVD_flag=0;
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
      //Bus_SendByte(uint8_t* sendbuf,uint8_t size);
  RTC_AWK_flag |= 1;
  RTC_ClearITPendingBit(RTC_IT_WUT);
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
  if (PWR_GetFlagStatus(PWR_FLAG_PVDOF) != RESET)//??????????????3.05V??
  {
    //??????????????
    PVD_flag |= 1;
    PWR_PVDClearITPendingBit();
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
  LED_ON;
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_FL_No_IT); 
  ext_flag |=1;
  s_bus.pre_start |= 1;                         //??????
  TIM3_Config();
  
  
  
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
    if(s_bus.pre_start){         //??4ms????????????????????????????????4ms????????????????
      s_bus.pre_start &= 0;         //??????????
      s_bus.start_flag &= 0;
      TIM3->CCER1 |= TIM_CCER1_CC1P;      //????????????????,
      if(ext_flag){s_bus.pre_start|=1;ext_flag=0; TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);}   //????????EXTI??????????????????????????
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
    if(test_count<70) record[test_count++] = TIM3_GetCapture1();  //??????70??????
    if(TIM3->CCER1>>1 & 1)                                             //????????????????????(??2????1) CC2P:00100000
    {
      
      //??????????????????????????
      if((!s_bus.start_flag)&&(!s_bus.pre_start))       //????1??????????????????4ms??????????????????????????????????????????????????
      {
        s_bus.pre_start |= 1;                  //??pre_start??1
        TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);      //??????????????
      }
      else if(s_bus.pre_start && s_bus.start_flag)        //????2??????????????????????
      {
        s_bus.high_time=TIM3_GetCapture1();     //??????????????????
        if(s_bus.high_time>40 && s_bus.high_time<170)//????????????????????????????,????????????80  160
        {
          TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);    //??????????????
          //??????????
        }
        else
        {
          s_bus.pre_start &=0;       //????????
          s_bus.start_flag &=0;
          s_bus.high_time = 0;
          //????????????????????????????????4ms????????
        }
        
        
      }
    }
     else                                                       //????????????????
    {      
      s_bus.low_time=TIM3_GetCapture1();                   //????????????????

      if(s_bus.pre_start && (!s_bus.start_flag))              //A??????????????????????????????????????????4ms????????????????????
      {
        if(s_bus.low_time< 3500 || s_bus.low_time>4400)                                            //a ????????????????????????3800us ??????4400????????????????????????????????0
        {
          s_bus.pre_start &=0;
          s_bus.start_flag &=0;
        }
        else                                                              //b ????????????????????3800??????4200us
        {
          s_bus.start_flag |= 1;                                             
        }
      }
      else if(s_bus.start_flag && s_bus.pre_start)        //B????????????  ???????????????????? 
      {
        if(s_bus.high_time<80 && s_bus.high_time>30&&s_bus.low_time<160 && s_bus.low_time>110)           //???????????? ??????40-70 120-150
        {
          rec_data[s_bus.datasum] &= (uint8_t)~(1<<s_bus.count);     //??count??????0
          s_bus.count++;
          if(s_bus.count==8)
          {
            //if(rec_data[0]==CMD_START) s_bus.datasum=1;         //??????????????????????????????rec_data[1]????????
            s_bus.datasum++;
            s_bus.count=0; 
            s_bus.pre_start&=0;
            s_bus.start_flag&=0;
          }   //??????????????????????????????????????????????
        }
        else if(s_bus.high_time>110 && s_bus.high_time<160 && s_bus.low_time<80 && s_bus.low_time>30)      //b????????140us??????????????100us???? ??1
        {
          rec_data[s_bus.datasum] |= (uint8_t)1<<s_bus.count;        //??count????1
          s_bus.count++;
          if(s_bus.count==8)
          {
            //if(rec_data[0]==CMD_START) s_bus.datasum=1;         //??????????????????????????????rec_data[1]????????
            s_bus.datasum++;
            s_bus.count=0;
            s_bus.pre_start&=0;
            s_bus.start_flag&=0;
          }   //??????????????????????????????????????????????
        }
        else                                                    //c???????? ????????????
        {
          s_bus.pre_start &= 0;
          s_bus.start_flag &= 0;
          s_bus.count=0;
        }
      }
    TIM3->CCER1 |= TIM_CCER1_CC1P;     //????????????????
    }
    TIM3_SetCounter(0x00);                  //????????????????
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
   //USART_ClearITPendingBit (USART1,USART_IT_RXNE);//??????????
   //USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//??????????????????????
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