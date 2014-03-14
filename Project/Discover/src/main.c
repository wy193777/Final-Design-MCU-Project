
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_it.c"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM2_PERIOD             65535
#define TIM2_PRESCALER              39999
#define TIM2_REPTETION_COUNTER      0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t CCR1_Val = 30000;
__IO uint16_t CCR2_Val = 16384;
__IO uint16_t CCR3_Val = 8192;

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM2_Config(void);
static void USART_Config(void);

void USART_SendByte(uint8_t data);
void USART_SendString(uint8_t* Data, uint16_t len);
uint8_t USART_ReceiveByte(void);
void Delay(int);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   
  CLK_Config(); 
  GPIO_Config();   
  TIM2_Config();  
  USART_Config();
  
  enableInterrupts();
  
  USART_SendString("Test\n", sizeof("Test\n"));
  //delay_10us(100);
  //USART_SendByte("\n");
  Delay(100);
  uint8_t Buf = 0;
  
  
  while (1)
  {
//    USART_SendString("Test\n", sizeof("Test\n"));
//    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    
//    Buf=USART_ReceiveData8(USART1);
//    while(USART_GetFlagStatus(USART1, USART_FLAG_OR) == SET);
//    if(Buf != 0)
//    {
//      USART_SendData8(USART1, Buf);
//    }
  }
}

/**
  * @brief  Configure peripherals 
  */
static void CLK_Config(void)
{
  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
}

static void GPIO_Config(void)
{
  /* GPIOD configuration: TIM1 channel 1 (PD2), channel 2 (PD4) and channel 3 (PD5) */
  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);  //Text int
  GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);  //Receive int
  //GPIO_SetBits(GPIOE, GPIO_Pin_7);
}

static void TIM2_Config(void)
{
  /*
  - TIM1CLK = 2 MHz
  - TIM1 counter clock = TIM1CLK / TIM1_PRESCALER+1 = 2 MHz/39999+1 = 50 Hz
  */
  /*
  - The TIM1 CCR1 register value is equal to 3000: 
  - So the TIM1 Channel 1 generates a periodic signal with a frequency equal to 50 Hz.
  - PB0
  */
  /* PWM1 Mode configuration: Channel1 */
  TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, 39999);
  TIM2_OC1Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, 0, TIM2_OCPolarity_High, TIM2_OCIdleState_Set);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_CtrlPWMOutputs(ENABLE);
  TIM2_SetCompare1(3000);
  TIM2_Cmd(ENABLE);
}

static void USART_Config(void)
{
  //PC3 Send
  //PC2 receive
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);   //Map USART to PortA
  
  GPIO_Init(GPIOA,GPIO_Pin_2,GPIO_Mode_In_PU_No_IT);    //USART_RX
  //GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Fast);     //USART_TX
  GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);        //Let this port been controlled by external signal
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
  //GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2|GPIO_Pin_3, ENABLE);
  USART_DeInit(USART1);  
  USART_Init(USART1,
             9600, 
             USART_WordLength_8b,
             USART_StopBits_1,
             USART_Parity_No,
             (USART_Mode_TypeDef)(USART_Mode_Rx|USART_Mode_Tx));
  USART_ClockInit(USART1,USART_Clock_Disable,USART_CPOL_Low,USART_CPHA_2Edge,USART_LastBit_Disable);
  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  //USART_ITConfig(USART1, (USART_FLAG_TypeDef)(USART_IT_TXE | USART_IT_RXNE), ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
}


void USART_SendByte(uint8_t data)
{
  USART_SendData8(USART1, (unsigned char)data);
  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void USART_SendString(uint8_t* Data, uint16_t len)
{
  //unit16_t i = 0;
  for(int i = 0; i < len; i++)
  {
    USART_SendByte(Data[i]);  
  }
}

uint8_t USART_ReceiveByte(void)
{
  uint8_t USART_RX_BUF;
  while(USART_GetFlageStatus(USART1, USART_FLAG_RXNE) == RESET);
  USART_RX_BUF = USART_ReceiveData8(USART1);
  return USART_RX_BUF;
}

void Delay(int time)
{
  for(int i = 0; i < time; i++)
    for(int j = 0; j < 100; j++);
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
