
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_it.c"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM2_PERIOD             65535
#define TIM2_PRESCALER              39999
#define TIM2_REPTETION_COUNTER      0
#define BUFFER_SIZE  10   
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t CCR1_Val = 30000;
__IO uint16_t CCR2_Val = 16384;
__IO uint16_t CCR3_Val = 8192;

extern unsigned char usart_buffer[BUFFER_SIZE];
extern uint16_t current_buffer_size;
extern uint16_t current_buffer_position;
extern short degree_interval;                 //store degree interval info
extern short time_interval;                   //store time interval number
extern bool config_complete_flag;             //true if config transmited 
extern bool working_flag;                     //true if servo motor under working
extern unsigned int time_interval_counter;    //count when 100 ms interrupt occur
extern int servo_motor_position;              //store the current degree of servo motor
extern int degree_interval_stored;  
extern const uint8_t ERROR_MESSAGE[5] = "ERROR";

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM2_Config(void);
static void TIM3_Config(void);
static void USART_Config(void);

extern void USART_SendByte(uint8_t data);
extern void USART_SendString(uint8_t* Data, uint16_t len);
extern uint8_t USART_ReceiveByte(void);
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
  TIM3_Config();       //to generate 100 ms interrupt
  TIM2_Config();       //to generate PWM to control servo motor
  USART_Config();
  
  
  uint8_t Buf = 0;
  working_flag = FALSE;
  config_complete_flag = FALSE;
  time_interval_counter = 0;
  servo_motor_position = 1000;
  
  enableInterrupts();
  
  while (1)
  {
    if(!working_flag)
    {
      if(config_complete_flag)
      {
        TIM2_SetCompare1(servo_motor_position);
        //Delay(1000);
        degree_interval_stored = 4000 / degree_interval;
        working_flag = TRUE;
        //TIM3_Cmd(ENABLE);
      } 
    }
  }
}

/**
  * @brief  Configure peripherals 
  */
static void CLK_Config(void)
{
  /* Enable TIM1 clock */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
}

static void GPIO_Config(void)
{
  /* GPIOD configuration: TIM1 channel 1 (PD2), channel 2 (PD4) and channel 3 (PD5) */
  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);  
  GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
  //GPIO_SetBits(GPIOE, GPIO_Pin_7);
}

static void TIM2_Config(void)
{
  /*
  - TIM2CLK = 2 MHz
  - TIM2 counter clock = TIM1CLK / TIM1_PRESCALER+1 = 2 MHz/39999+1 = 50 Hz
  - x / 40000 * 20 = time of high (ms)
  - servo motor 0.5 ms -> 0   degree x = 1000
  - servo motor 1.0 ms -> 45  degree x = 2000
  - servo motor 1.5 ms -> 90  degree x = 3000
  - servo motor 2.0 ms -> 135 degree x = 4000
  - servo motor 2.5 ms -> 180 degree x = 5000
  */
  /*
  - The TIM2 CCR1 register value is equal to 3000: 
  - So the TIM2 Channel 1 generates a periodic signal with a frequency equal to 50 Hz.
  - PB0
  */
  /* PWM1 Mode configuration: Channel1 */
  TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, 39999);
  TIM2_OC1Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, 0, TIM2_OCPolarity_High, TIM2_OCIdleState_Set);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_CtrlPWMOutputs(ENABLE);
  TIM2_SetCompare1(3000);           //on 90 degree
  TIM2_Cmd(ENABLE);
}


static void TIM3_Config(void)
{
  /*
  - interrupt every 100 ms
  - let servo motor behave every time_interval * 100 ms
  - 2M/(Prescaler*3125) = 10Hz
  - 
  */
  
  TIM3_TimeBaseInit(TIM3_Prescaler_64, TIM3_CounterMode_Up, 3125);
  TIM3_SetAutoreload(3125);
  TIM3_ITConfig(TIM3_IT_Update, ENABLE);
  TIM3_ARRPreloadConfig(ENABLE);
  //TIM3_UpdateDisableConfig(ENABLE);
  TIM3_Cmd(ENABLE);
  
  /*
  TIM1_TimeBaseInit(64, TIM1_CounterMode_Up, 3125, 0);
  TIM1_SetAutoreload(3125);
  TIM1_ITConfig(TIM1_IT_Update, ENABLE);
  TIM1_Cmd(ENABLE);
  */
}


static void USART_Config(void)
{
  //PA3 Send
  //PA2 receive
  current_buffer_size = 0;
  current_buffer_position = 0;
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
