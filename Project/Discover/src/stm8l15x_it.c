/**
  ******************************************************************************
  * @file    Project/Template/stm8l15x_it.c
  * @author  MCD Tools Team
  * @version V1.2.0
  * @date    09/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "discover_functions.h"
#include "discover_board.h"
#include "stm8l_discovery_lcd.h"

/*extern bool KeyPressed;
extern uint8_t state_machine;
extern bool Auto_test;
extern uint8_t t_bar[2];*/
void USART_SendString(uint8_t* Data, uint16_t len);
/** @addtogroup IT_Functions
  * @{
  */
uint16_t const BUFFER_SIZE = 10;
uint8_t const ERROR_MESSAGE[5];
unsigned char usart_buffer[];
uint16_t current_buffer_position;
uint16_t current_buffer_size;
short degree_interval;
short time_interval;
bool config_complete_flag;
bool working_flag;
unsigned int time_interval_counter;
int servo_motor_position;              //store the current degree of servo motor
int degree_interval_stored;  

 
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PORTB Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTIB_IRQHandler,6)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PORTD Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTID_IRQHandler,7)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief External IT PIN1 Interrupt routine.
  *   On User button pressed:
  *   Check if button presse a long time (4-5 sec.) if yes --> Set Autotest
  *   Else update status_machine to pass to next measuremetn.
  *   Update the LCD bar graph
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{

}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
  
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
//  disableInterrupts();
//  
//  /* To clear External IT Pin6 */
//  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
//  
//  enableInterrupts();

}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  
  while (1);

}


/**
  * @brief TIM2 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler,19)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM2_CAP_IRQHandler,20)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_IRQHandler,21)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  //while (1);
  if(working_flag)
  {
    time_interval_counter += 1;
    if(time_interval_counter < time_interval)
    {//wait until next interrupt
    
    }
    else
    {//let servo motor turn a small degree
      time_interval_counter = 0;
      if(servo_motor_position < 5000)
      {
        servo_motor_position += degree_interval_stored;  
      }
      else
      {
        servo_motor_position = 5000;
      }
      TIM2_SetCompare1(servo_motor_position);
    }  
  }
  TIM3_ClearITPendingBit(TIM3_IT_Update);
}
/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM3_CAP_IRQHandler,22)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM1_CAP_IRQHandler,24)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief SPI1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief USART1 TX Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(USART1_TX_IRQHandler,27)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/GPIO_SetBits(GPIOC, GPIO_Pin_7);
  //while (1);

}

/**
  * @brief USART1 RX Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  * If receive a long series of data, this could excute every 
  * bit received.
  */
INTERRUPT_HANDLER(USART1_RX_IRQHandler,28)
{
  unsigned char Buf;
  Buf = USART_ReceiveData8(USART1);
  if(Buf == 'D')
  {
    usart_buffer[0] = Buf;
    current_buffer_position = 1;  
    config_complete_flag = FALSE;
  }
  else if(Buf == 'T')
  {
    usart_buffer[2] = Buf;
    current_buffer_position = 3;  
    config_complete_flag = FALSE;
  }
  else if(Buf != 'D' && Buf != 'T' && current_buffer_position == 1)
  {
    usart_buffer[1] = Buf;
    //degree_interval = Buf; 
    config_complete_flag = FALSE;
  }
  else if(Buf != 'D' && Buf != 'T' && current_buffer_position == 3)
  {
    usart_buffer[3] = Buf;
    //time_interval = Buf; 
    config_complete_flag = TRUE;
    current_buffer_position = 0;
  }
  else
  {
    config_complete_flag = FALSE;
    current_buffer_position = 0;
  }
  degree_interval = usart_buffer[1]; 
  time_interval = usart_buffer[3];
  //while (1);
  //RXNE flag couldn't been cleared by automatically if 
  //ReceiveData wasn't been called
  
  //USART_ClearFlag(USART1, USART_FLAG_RXNE);    
}

/**
  * @brief I2C1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(I2C1_IRQHandler,29)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}

/**
  * @}
  */
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/


/* This part of code is to heavy to use
 *
INTERRUPT_HANDLER(USART1_RX_IRQHandler,28)
{
  uint8_t Buf;
//  GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
//  Buf = USART_ReceiveData8(USART1);
//  USART_SendData8(USART1, Buf);
  //while (1);
  //disableInterrupts();
  /*uint8_t data_size = 0;
  bool transmit_right_flag = FALSE; //set to true when receive finished and right
  int sum = 0;
  
  usart_buffer[current_buffer_position] = USART_ReceiveData8(USART1);
  
  
  //protocal
  if(current_buffer_position >= 2 && 
     usart_buffer[0] == 0x53 &&     //"STA", the beginning of protocal .
     usart_buffer[1] == 0x54 &&
     usart_buffer[2] == 0x41)
  {
    //When head right
    //USART_SendString("HEAD_RIGHT", sizeof("HEAD_RIGHT"));
    if(current_buffer_position > BUFFER_SIZE)
    {
      current_buffer_position = 0;
    }
    if(current_buffer_position>= 3)
    {
      data_size = usart_buffer[3];  
    }
    if(current_buffer_position == (3 + data_size +4) &&
       usart_buffer[3 + data_size + 2] == 0x45 &&   //"END", the tail of protocal
       usart_buffer[3 + data_size + 3] == 0x4E &&
       usart_buffer[3 + data_size + 4] == 0x44)
    {
      current_buffer_position = 0;
      transmit_right_flag = TRUE;
      //USART_SendString("END_FINISH", sizeof("END_FINISH"));  
    }
    else if(current_buffer_position == 3 + data_size + 4)
    {
      //USART_SendString("END_ERROR", sizeof("END_ERROR")); 
      current_buffer_position = 0;
    }
  }
  else if(current_buffer_position >= 2)
  {
    if(usart_buffer[0] != 0x53 ||     //When head has been sent but not so right .
       usart_buffer[1] != 0x54 ||
       usart_buffer[2] != 0x41)
    {
      //USART_SendString("START_ERROR", sizeof("START_ERROR")); 
      current_buffer_position = 0;
    }
  }
  
  //Judge the rightness of data
  if(transmit_right_flag)
  {
    for(int i = 1; i <= data_size; i++)
    {
      sum += usart_buffer[3 + i];   
    }
    if(sum == usart_buffer[3 + data_size + 1])
    {
      //USART_SendString("Succeed!", sizeof("Succeed!")); 
    }
    else
    {
      //USART_SendString("Check failed!", sizeof("Check failed!")); 
    }
  }
  current_buffer_position++;
  //enableInterrupts();
  //USART_SendByte(0x53);  
  //USART_SendString("C", sizeof("C")); 
  //RXNE flag couldn't been cleared by automatically if 
  //ReceiveData wasn't been called
  
  USART_ClearFlag(USART1, USART_FLAG_RXNE);    
}
*/