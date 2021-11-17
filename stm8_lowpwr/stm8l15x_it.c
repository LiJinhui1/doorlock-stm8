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
uint8_t lpwrflg = CMD_LWPWR_FLAG;
bool ext_flag=0;
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
  if (PWR_GetFlagStatus(PWR_FLAG_PVDOF) != RESET)//捕获到电压小于3.05V了
  {
    //唤醒，发送信号
    EXTI_DeInit();
    Bus_SendByte(&lpwrflg,1);  //发送低电量信号，之后就进行while的主函数了
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
  
  GPIO_Init(SINGLEBUS_PORT,SINGLEBUS_PINS,GPIO_Mode_In_FL_No_IT); 
  TIM3_Config();
  ext_flag |=1;
  s_bus.pre_start |= 1;                         //预接收
  
  
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
    if(s_bus.pre_start){         //在4ms等待过程中和传输过程中溢出，等了4ms后还是没有上升沿
      s_bus.pre_start &= 0;         //标志位清除
      s_bus.start_flag &= 0;
      TIM3->CCER1 |= TIM_CCER1_CC1P;      //修改当前为下降沿,
      if(ext_flag){s_bus.pre_start|=1;ext_flag=0; TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);}
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
    if(test_count<70) record[test_count++] = TIM3_GetCapture1();  //记录前30个数据
    if(TIM3->CCER1>>1 & 1)                                             //如果当前是下降沿触发(第2位是1) CC2P:00100000
    {
      
      //唤醒情况下进入的下降沿中断
      if((!s_bus.start_flag)&&(!s_bus.pre_start))       //状态1：如果当前没有进入4ms等待，没有正在传输，有下降沿，那么现在是拉低的开始
      {
        s_bus.pre_start |= 1;                  //将pre_start置1
        TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);      //设置上升沿触发
      }
      else if(s_bus.pre_start && s_bus.start_flag)        //状态2：当前是进入了传输过程
      {
        s_bus.high_time=TIM3_GetCapture1();     //取出捕获寄存器的值
        if(s_bus.high_time>40 && s_bus.high_time<200)//判断当前的高电平时间是否正常,不正常就重置80  160
        {
          TIM3->CCER1 &= (uint8_t)(~TIM_CCER1_CC1P);    //设置上升沿触发
          //并且入数据
        }
        else
        {
          s_bus.pre_start &=0;       //标志清空
          s_bus.start_flag &=0;
          s_bus.high_time = 0;
          //不修改触发极性，还是下降沿，等待4ms拉低信号
        }
        
        
      }
    }
     else                                                       //当前是上升沿触发
    {      
      
      s_bus.low_time=TIM3_GetCapture1();                   //记录当前的时间戳   
      if(s_bus.pre_start && (!s_bus.start_flag))              //A已经有下降沿但是这是第一个上升沿，就是等待4ms的时候来了一个上升沿
      {
        if(s_bus.low_time< 3600 || s_bus.low_time>4400)                                            //a 如果当前的低电平时间小于3800us （大于4400就溢出了），溢出时标志位就全部置0
        {
          s_bus.pre_start &=0;
          s_bus.start_flag &=0;
        }
        else                                                              //b 当前的低电平时间大于3800，小于4200us
        {
          s_bus.start_flag |= 1;                                             
        }
      }
      else if(s_bus.start_flag && s_bus.pre_start)        //B判断当前状态  现在是进入传输过程了 
      {
        
        if(s_bus.high_time<120 && s_bus.high_time>40&&s_bus.low_time<200 && s_bus.low_time>120)           //判断电平时间 区间最大化80和160 40的允许误差
        {
          data[s_bus.datasum] &= (uint8_t)~(1<<s_bus.count);     //第count位变为0
          s_bus.count++;
          if(s_bus.count==8){s_bus.count=0; s_bus.datasum++;}
        }
        else if(s_bus.high_time>120 && s_bus.high_time<200 && s_bus.low_time<120 && s_bus.low_time>40)      //b高电平在140us以上，低电平在100us以下 入1
        {
          data[s_bus.datasum] |= (uint8_t)1<<s_bus.count;        //第count位置1
          s_bus.count++;
          if(s_bus.count==8){s_bus.count=0; s_bus.datasum++;}
        }
        else                                                    //c其他情况 恢复默认状态
        {
          s_bus.pre_start &= 0;
          s_bus.start_flag &= 0;
        }
      }
    TIM3->CCER1 |= TIM_CCER1_CC1P;     //修改当前为下降沿
    }
    TIM3_SetCounter(0x00);                  //将当前计数值复位
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
   //USART_ClearITPendingBit (USART1,USART_IT_RXNE);//清中断标志
   //USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//将接收到的数据发送出去
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