
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM1_PWMOutput
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM1_PERIOD             65535
#define TIM1_PRESCALER              39999
#define TIM1_REPTETION_COUNTER      0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t CCR1_Val = 30000;
__IO uint16_t CCR2_Val = 16384;
__IO uint16_t CCR3_Val = 8192;

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM1_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   /* CLK configuration -------------------------------------------*/
  CLK_Config(); 

   /* GPIO configuration -------------------------------------------*/
  GPIO_Config(); 
  
   /* TIM1 configuration -------------------------------------------*/
  TIM1_Config();

  /* Infinite loop */
  while (1)
  {}
}

/**
  * @brief  Configure peripherals 
  */
static void CLK_Config(void)
{
  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
}

static void GPIO_Config(void)
{
  /* GPIOD configuration: TIM1 channel 1 (PD2), channel 2 (PD4) and channel 3 (PD5) */
  GPIO_Init(GPIOD, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
  //GPIO_SetBits(GPIOE, GPIO_Pin_7);
}

/**
  * @brief  Configure TIM1 peripheral 
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
  /*
  - TIM1CLK = 2 MHz
  - TIM1 counter clock = TIM1CLK / TIM1_PRESCALER+1 = 2 MHz/39999+1 = 50 Hz
  */
  /* Time base configuration */
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);

  /*
  - The TIM1 CCR1 register value is equal to 3000: 
  - So the TIM1 Channel 1 generates a periodic signal with a frequency equal to 50 Hz.
  - PD2
  */
  /* PWM1 Mode configuration: Channel1 */
  TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               CCR1_Val, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_SetCompare1(15000);
  TIM1_OC1PreloadConfig(ENABLE);

  /*
  - The TIM1 CCR2 register is equal to 16384:
  - CC2 update rate = TIM1 counter clock / CCR2_Val = 61.03 Hz
  - So the TIM1 channel 2 generates a periodic signal with a frequency equal to 30.51 Hz.
  */
  /* Toggle Mode configuration: Channel2 */
  TIM1_OC2Init(TIM1_OCMode_Toggle, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               CCR2_Val, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC2PreloadConfig(DISABLE);

  /*
  -The TIM1 CCR3 register is equal to 8192:
  - CC3 update rate = TIM1 counter clock / CCR3_Val = 123.01 Hz
  - So the TIM1 channel 3 generates a periodic signal with a frequency equal to 61.5 Hz.
  */
  /* Toggle Mode configuration: Channel3 */
  TIM1_OC3Init(TIM1_OCMode_Toggle, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               CCR3_Val, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC3PreloadConfig(DISABLE);

  TIM1_ARRPreloadConfig(ENABLE);

  /* TIM1 Interrupt enable */
  TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
  TIM1_ITConfig(TIM1_IT_CC2, ENABLE);
  TIM1_ITConfig(TIM1_IT_CC3, ENABLE);
  enableInterrupts();

  /* Enable TIM1 outputs */
  TIM1_CtrlPWMOutputs(ENABLE);
  /* TIM1 enable counter */
  TIM1_Cmd(ENABLE);
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
