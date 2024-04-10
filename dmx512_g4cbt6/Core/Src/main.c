/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t uartBuff1[514];
volatile uint8_t uartBuff2[515];
volatile uint8_t uartBuff3[515];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	for(int i=0;i<514;i++)
					{
						uartBuff1[i]=0;
					}
			uartBuff1[0]=222; //Testovací byty
			uartBuff1[1]=255;
			uartBuff1[2]=255;
			uartBuff1[3]=20;
			uartBuff1[506]=1;
			uartBuff1[507]=2;
			uartBuff1[508]=3;
			uartBuff1[509]=4;
			uartBuff1[510]=5;
			uartBuff1[511]=255;
			uartBuff1[512]=255;
	for(int i=0;i<513;i++)
						{
							uartBuff3[i]=0;
						}
			uartBuff3[0]=0; //Testovací byty
			uartBuff3[1]=255;
			uartBuff3[2]=255;
			uartBuff3[3]=20;
			uartBuff3[506]=1;
			uartBuff3[507]=2;
			uartBuff3[508]=3;
			uartBuff3[509]=4;
			uartBuff3[510]=5;
			uartBuff3[511]=255;
			uartBuff3[512]=255;
			uartBuff3[513]=50;
			for(int i=0;i<515;i++)
					{
						uartBuff2[i]=0;
					}
			uartBuff2[0]=0xAA;
			uartBuff2[1]=0; //Testovací byty
				uartBuff2[2]=253;
				uartBuff2[3]=254;
				uartBuff2[4]=255;
				uartBuff2[507]=1;
				uartBuff2[508]=2;
				uartBuff2[509]=3;
				uartBuff2[510]=4;
				uartBuff2[511]=5;
				uartBuff2[512]=6;
				uartBuff2[513]=7;
				uartBuff2[514]=0xBB;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init();//initial driver setup to drive ili9341
  SwitchToTransmit();
  HAL_TIM_Base_Start_IT(&htim2); //Zahájí časovač na odesílání DMX512
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, uartBuff1, 514); //Začne přijímání DMX512 (musí být dvakrát, jinak se nechytí vždy)
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, uartBuff1, 514); //TODO: Při čtení cizího DMX signálu nepřečte první nultej byt a tím pádem nedopíše poslední byte z DMXka, na starým to fungovalo, je to chyba HALu asi
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_UART_Transmit_IT(&huart3, uartBuff1, 513);
		  ILI9341_Fill_Screen(RED);
		  	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		  	  		ILI9341_Draw_Text("Dotykova obrazovka", 10, 10, BLACK, 2, WHITE);
		  	  		ILI9341_Draw_Text("Muzete kreslit", 10, 30, GREEN, 2, WHITE);
		  	  		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		  	  		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		  	  		while(1)
		  	  		{
		  	  			if(TP_Touchpad_Pressed())
		  	          {

		  	  					uint16_t x_pos = 0;
		  	  					uint16_t y_pos = 0;


		  	            uint16_t position_array[2];

		  	  					if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
		  	  					{

		  	  						x_pos = position_array[0];
		  	  						y_pos = position_array[1];

		  	  						ILI9341_Draw_Filled_Circle(x_pos, y_pos, 2, BLACK);

		  	  						ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		  	  						char counter_buff[30];
		  	  						sprintf(counter_buff, "POZ X: %.3d", x_pos);
		  	  						ILI9341_Draw_Text(counter_buff, 10, 80, BLACK, 2, WHITE);
		  	  						sprintf(counter_buff, "POZ Y: %.3d", y_pos);
		  	  						ILI9341_Draw_Text(counter_buff, 10, 120, BLACK, 2, WHITE);


		  	  						ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		  	  					}



		  	          }
		  	  		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 75;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void SwitchToTransmit()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}
void SwitchToReceiveOnly()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) //Po prijmuti celeho paketu:
{

	HAL_UARTEx_ReceiveToIdle_IT(&huart2, uartBuff1, 513);
	for(int i=512; i!=0; i--)
		{
			uartBuff3[i+1]=uartBuff1[i];
		}
	//uartBuff1[0]=0;
	/*if(HAL_UARTEx_GetRxEventType(huart)==HAL_UART_RXEVENT_IDLE)
	{
		for(int i=512; i>=0; i--)
		{
		uartBuff3[i+1]=uartBuff1[i];
		}
		HAL_UARTEx_ReceiveToIdle_IT(&huart1, uartBuff1, 513);//Zacit prijimat ------------ pokud visi ve vzduchu, muze prijimat nesmysly //!! Pokud nepřijmul všech 512 bytů, přidat nulu
	}
	else if(HAL_UARTEx_GetRxEventType(huart)==HAL_UART_RXEVENT_TC) //todo idle line
	{
	memcpy(uartBuff3, uartBuff1, 513 * sizeof(int));
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, uartBuff1, 513);
	//Zacit prijimat ------------ pokud visi ve vzduchu, muze prijimat nesmysly //!! Pokud nepřijmul všech 512 bytů, přidat nulu
	}
	//HAL_UART_Transmit_IT(&huart1, uartBuff1, 513); <- pak by to mělo přijít sem, ale zatím to dám do časovače*/
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) //Po doběhnutí časovače
{
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_SET);
	uint32_t i=8*4000; //Timer na 1000=mikrosekundy <---------------------
	//while(i--);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_RESET);
	//i=8*4000; //Timer na 1000=mikrosekundy <---------------------
	//	while(i--);
	SwitchPin_ToMode_UART(); //Přepne pin do režimu UART
	i=8*50; //POVOLIT
	while(i--); //POVOLIT
	//i=8*1000; //Timer na 1000=mikrosekundy <---------------------
	//while(i--);
	//Přidat MAB...
	//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); //TODO: Nastavit délku MAB (dle osciloskopu)
	//uartBuff1[0]=222;
	HAL_UART_Transmit_IT(&huart2, uartBuff3, 513); //TODO: sizeof(uartBuff1)
	uartBuff1[513]=50;
	HAL_UART_Transmit_IT(&huart1, uartBuff3, 514);
	//uartBuff1[3]=uartBuff1[3]+1;
	//HAL_UART_Transmit_IT(&huart1, uartBuff2, 515);
	//uartBuff2[1]=uartBuff2[1]+1;
	//HAL_UART_Transmit_IT(&huart1, "\n", sizeof("\n"));
	//HAL_UARTEx_ReceiveToIdle_IT(&huart1, uartBuff1, 513); //Může tu být, z definice stejně nezačne přijímat pokud už příjem běží...
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) //Po odeslání -- pokud uart2 -> přepnout na IDLE; pokud uart 1 -> nic
{
	//if(huart==&huart3) //TODO: Opravit pokud tu bude chyba; ale asi funguje
	//{

	   SwitchPin_ToMode_GPIO_Output();
	   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_RESET);
	  // uint32_t i=8*100; //Timer na 1000=mikrosekundy <---------------------
	   	//   while(i--);
	   //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_SET);
	   //i=8*500; //Timer na 1000=mikrosekundy <---------------------
	   //while(i--);
	   //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_RESET); //TODO: Pojmenovat podle jmen - zapomínáš to přepsat potom,

	   //uint32_t i=8*1000; //Timer na 1000=mikrosekundy
	   //	while(i--);
	//}
}

void SwitchPin_ToMode_GPIO_Output(void) //TODO: přidat nastaveni PINu k prepnuti
{
    GPIO_InitTypeDef GPIO_Initialize;
    GPIO_Initialize.Pin = GPIO_PIN_2;
    GPIO_Initialize.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initialize.Pull = GPIO_PULLUP;
    GPIO_Initialize.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initialize);
}
void SwitchPin_ToMode_UART(void)
{
    GPIO_InitTypeDef GPIO_Initialize;
    GPIO_Initialize.Pin = GPIO_PIN_2;
    GPIO_Initialize.Mode = GPIO_MODE_AF_PP;  //Alternate function
    GPIO_Initialize.Pull = GPIO_PULLUP;
    GPIO_Initialize.Alternate = GPIO_AF7_USART2; //Novinka pro toto MCU
    GPIO_Initialize.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initialize);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
