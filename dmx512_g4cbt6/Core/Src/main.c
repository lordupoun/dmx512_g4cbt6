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
volatile uint8_t receiveBuff[514]; //Paket přijatý z RS485/PC
volatile uint8_t sendToPC[520]; //paket pro zpracování PC aplikací
volatile uint8_t sendToDMX[513]; //standardní paket na RS485 rozhraní
uint8_t packetsSentAfterStart=0; //přepsat na uint8_t
int toReceive=514; //přepsat na uint16_t
uint8_t receiveMode=1;
UART_HandleTypeDef *uartRx; //výběr přijímacího uartu
uint8_t menuNav;
uint16_t valueOffset;
char str[10];
uint8_t sendingToDMX;

//Displej:
//int xmin;
//int xmax;
//int ymin;
//int ymax;
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
			receiveBuff[i]=0;
		}
	receiveBuff[0]=0;
	receiveBuff[1]=0;
	receiveBuff[2]=255;
	receiveBuff[3]=255;
	receiveBuff[4]=255;
	receiveBuff[5]=255;
	receiveBuff[506]=1;
	receiveBuff[507]=2;
	receiveBuff[508]=3;
	receiveBuff[509]=4;
	receiveBuff[510]=5;
	receiveBuff[511]=255;
	receiveBuff[512]=255;

	for(int i=0;i<515;i++)
		{
			sendToPC[i]=0;
		}
	sendToPC[0]=121;
	sendToPC[1]=122;
	sendToPC[2]=253;
	sendToPC[3]=254;
	sendToPC[4]=255;
	sendToPC[507]=1;
	sendToPC[508]=2;
	sendToPC[509]=3;
	sendToPC[510]=4;
	sendToPC[511]=5;
	sendToPC[512]=6;
	sendToPC[518]=131;
	sendToPC[519]=132;

	for(int i=0;i<513;i++)
		{
			sendToDMX[i]=0;
		}
	sendToDMX[0]=0;
	sendToDMX[1]=255;
	sendToDMX[2]=255;
	sendToDMX[3]=0;
	sendToDMX[4]=0;
	sendToDMX[5]=0;
	sendToDMX[506]=1;
	sendToDMX[507]=2;
	sendToDMX[508]=3;
	sendToDMX[509]=4;
	sendToDMX[510]=5;
	sendToDMX[511]=255;
	sendToDMX[512]=0;


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
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
  //ReceiveFrom_PC();
  //ReceiveFrom_DMX();
  ILI9341_Init();
  //SwitchToTransmit(); //TODO: přepínatelnej režim - vysílám/nevysílám, ale to až budu mít hotovou komunikaci s PC asi...; Případně vypnutí přijímání a vysílání úplně
  //HAL_TIM_Base_Start_IT(&htim2);
  //HAL_UART_Transmit_IT(&huart2, sendToDMX, 513);
  stopAll();
  uartRx = &huart2;
  uint16_t position_array[2];
  //Displejos: //TODO: Tlačítko pro ukončení přijímání (ostatní tlačítka jej zase zapnou (funkce ReceiveFrom_PC bude zapínat)). //Menu: Analyzátor DMX - Přijímač Only (přijíma z DMX); Generátor DMX - Vysílač Only (vysílá z PC); Modifikátor DMX - Přijímač a Vysílač - rele on (přijímá z DMX a PC); Přijímat z PC, Přijímat z DMX; Manuální ovládání - Barvy; Pod menu analýza manuální na zařízení

  /*ILI9341_Draw_Text("Test DMX512", 10, 10, BLACK, 2, WHITE);
  ILI9341_Draw_Filled_Rectangle_Coord(20, 60, 80, 120, RED);
 	 ILI9341_Draw_Filled_Rectangle_Coord(20, 140, 80, 200, GREEN);
 	 ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 80, 280, BLUE);
 	ILI9341_Draw_Filled_Rectangle_Coord(160, 60, 220, 120, PINK);
 	ILI9341_Draw_Filled_Rectangle_Coord(160, 140, 220, 200, LIGHTGREY);
 	ILI9341_Draw_Filled_Rectangle_Coord(160, 220, 220, 280, DARKGREY);*/
  Draw_MainMenu1_Horizontal();
  menuNav=1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
	{
	if(TP_Touchpad_Pressed())
	{
		uint16_t x_pos = 0;
		uint16_t y_pos = 0;

		if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
		{
			x_pos = -(position_array[1]-320);
			y_pos = position_array[0];

			if(x_pos>250&&y_pos>205&&x_pos<320&&y_pos<240&&menuNav==1) //Vpřed MainMenu
			{
				Draw_MainMenu2_Horizontal();
			}
			if(x_pos>0&&y_pos>205&&x_pos<70&&y_pos<240&&menuNav==2) //Zpátky MainMenu
			{
				Draw_MainMenu1_Horizontal();
			}
			if(x_pos>20&&y_pos>29&&x_pos<300&&y_pos<79&&menuNav==1)
			{
				menuNav=3;
				ILI9341_Fill_Screen(DARKGREY);
				Draw_NavigationBar_Left_Horizontal();
				ILI9341_Draw_Text("Analyza signalu", 3, 1, BLACK, 2, DARKGREY);
				ILI9341_Draw_Text("Rezim analyzy je zapnuty,", 5, 60, BLACK, 2, RED);
				ILI9341_Draw_Text("Pokracujte v PC aplikaci", 5, 85, BLACK, 2, RED);
				setTo_Analyze();
			}
			if(x_pos>20&&y_pos>85&&x_pos<300&&y_pos<135&&menuNav==1)
			{
				menuNav=3;
				ILI9341_Fill_Screen(DARKGREY);
				Draw_NavigationBar_Left_Horizontal();
				ILI9341_Draw_Text("Generovani signalu", 3, 1, BLACK, 2, DARKGREY);
				ILI9341_Draw_Text("Rezim generovani", 5, 60, BLACK, 2, RED);
				ILI9341_Draw_Text("je zapnuty,", 5, 80, BLACK, 2, RED);
				ILI9341_Draw_Text("Pokracujte v PC aplikaci", 5, 105, BLACK, 2, RED);
				setTo_GenerateOnPC();
			}
			if(x_pos>20&&y_pos>141&&x_pos<300&&y_pos<191&&menuNav==1)
			{
				menuNav=3;
				ILI9341_Fill_Screen(DARKGREY);
				Draw_NavigationBar_Left_Horizontal();
				ILI9341_Draw_Text("Modifikace signalu", 3, 1, BLACK, 2, DARKGREY);
				ILI9341_Draw_Text("Rezim modifikace", 5, 60, BLACK, 2, RED);
				ILI9341_Draw_Text("je zapnuty,", 5, 80, BLACK, 2, RED);
				ILI9341_Draw_Text("PC aplikace", 5, 105, BLACK, 2, RED);
				ILI9341_Draw_Text("jej zatim nepodporuje", 5, 125, BLACK, 2, RED);
				setTo_Modification();
			}
			if(x_pos>20&&y_pos>29&&x_pos<300&&y_pos<79&&menuNav==2) //Manualni ovladani page1
			{
				valueOffset=0;
				ILI9341_Fill_Screen(LIGHTGREY);
				Draw_NavigationBar_Left_Horizontal();
				ILI9341_Draw_Text("Manualni analyza", 3, 1, BLACK, 2, LIGHTGREY);
				Draw_NavigationBar_Right_Horizontal();
				char page[7];
				sprintf(page, " %d/29 ", valueOffset+1);
				ILI9341_Draw_Text(page, 136, 220, BLACK, 2, LIGHTGREY);
				menuNav=5;
				setTo_AnalyzeLocally();
			}
			if(x_pos>250&&y_pos>205&&x_pos<320&&y_pos<240&&menuNav==5) //Manualni ovladani page2
			{
				if(valueOffset<495)
				{
					valueOffset+=18;
					char page[7];
					sprintf(page, " %d/29 ", valueOffset/18+1);
					ILI9341_Draw_Text(page, 133, 220, BLACK, 2, LIGHTGREY);
					HAL_Delay(50);
				}
				else
				{
					ILI9341_Fill_Screen(LIGHTGREY);
					Draw_NavigationBar_Left_Horizontal();
					ILI9341_Draw_Text("Manualni analyza", 3, 1, BLACK, 2, LIGHTGREY);
					Draw_NavigationBar_Right_Horizontal();
					char page[7];
					sprintf(page, " %d/29 ", valueOffset/18+1);
					ILI9341_Draw_Text(page, 133, 220, BLACK, 2, LIGHTGREY);
					HAL_Delay(50);
				}
			}
			if(x_pos>20&&y_pos>85&&x_pos<300&&y_pos<135&&menuNav==2)
			{
				menuNav=4;
				ILI9341_Fill_Screen(DARKGREY);
				Draw_NavigationBar_Left_Horizontal();
				ILI9341_Draw_Text("Manualni generovani", 0, 0, BLACK, 2, DARKGREY);
				setTo_GenerateLocally();
			}
			if(x_pos>0&&y_pos>205&&x_pos<70&&y_pos<240&&menuNav==3) //Zpátky do page1 menu z podmenu
			{
				menuNav=1;
				Draw_MainMenu1_Horizontal();
				stopAll();
			}
			if(x_pos>0&&y_pos>205&&x_pos<70&&y_pos<240&&menuNav==4) //Zpátky do page2 menu z podmenu
			{
				menuNav=2;
				Draw_MainMenu2_Horizontal();
				stopAll();
			}
			if(x_pos>0&&y_pos>205&&x_pos<70&&y_pos<240&&menuNav==5) //Zpátky SUB menu
			{
				if(valueOffset==0)
				{
					menuNav=2;
					Draw_MainMenu2_Horizontal();
				}
				else
				{
					valueOffset-=18;
					char page[7];
					sprintf(page, " %d/28 ", valueOffset/18+1);
					ILI9341_Draw_Text(page, 133, 220, BLACK, 2, LIGHTGREY);
					HAL_Delay(50);
				}
			}
			/*menuNav=1;
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Fill_Screen(DARKGREY);
			ILI9341_Draw_Text("DMX512 ANALYZATOR", 58, 5, BLACK, 2, DARKGREY);
			ILI9341_Draw_Filled_Rectangle_Coord(20, 29, 300, 79, LIGHTGREY); //+10
			ILI9341_Draw_Text("Analyza signalu", 68, 44, BLACK, 2, LIGHTGREY); //+5
			ILI9341_Draw_Filled_Rectangle_Coord(20, 85, 300, 135, LIGHTGREY);
			ILI9341_Draw_Text("Generovani signalu", 48, 100, BLACK, 2, LIGHTGREY);
			ILI9341_Draw_Filled_Rectangle_Coord(20, 141, 300, 191, LIGHTGREY);
			ILI9341_Draw_Text("Modifikace signalu", 48, 156, BLACK, 2, LIGHTGREY);
			ILI9341_Draw_Filled_Rectangle_Coord(20, 172, 300, 212, LIGHTGREY);
			ILI9341_Draw_Text("Manualni analyza", 22, 172, BLACK, 2, LIGHTGREY);
			ILI9341_Draw_Filled_Rectangle_Coord(20, 218, 300, 258, LIGHTGREY);
			ILI9341_Draw_Text("Manualni gen.", 38, 218, BLACK, 2, LIGHTGREY);
			ILI9341_Draw_Text("1/2", 145, 220, BLACK, 2, DARKGREY);
			Draw_NavigationBar_Right_Horizontal();*/
			 /* if(x_pos>20&&x_pos<80&&y_pos>60&&y_pos<120)
			  {
				  sendToDMX[2]=255;
				  sendToDMX[3]=0;
				  sendToDMX[4]=0;
				  sendToDMX[5]=0;
				  ILI9341_Draw_Hollow_Rectangle_Coord(18, 58, 82, 122, WHITE);
				  xmin=18;
				  xmax=82;
				  ymin=58;
				  ymax=122;
				  HAL_TIM_Base_Start_IT(&htim17); //Místo časovače dát permanentní volítko barev

			  }
			  if(x_pos>20&&x_pos<80&&y_pos>140&&y_pos<200)
			  {
				  sendToDMX[2]=0;
				  sendToDMX[3]=255;
				  sendToDMX[4]=0;
				  sendToDMX[5]=0;
				  ILI9341_Draw_Hollow_Rectangle_Coord(18, 138, 82, 202, WHITE);
				  xmin=18;
				  xmax=82;
				  ymin=138;
				  ymax=202;
				  HAL_TIM_Base_Start_IT(&htim17);
			  }
			  if(x_pos>20&&x_pos<80&&y_pos>220&&y_pos<280)
			  {
				  sendToDMX[2]=0;
				  sendToDMX[3]=0;
				  sendToDMX[4]=255;
				  sendToDMX[5]=0;
				  ILI9341_Draw_Hollow_Rectangle_Coord(18, 218, 82, 282, WHITE);
				  xmin=18;
				  xmax=82;
				  ymin=218;
				  ymax=282;
				  HAL_TIM_Base_Start_IT(&htim17);
			  }
			  if(x_pos>160&&x_pos<220&&y_pos>60&&y_pos<120)
			  {
				  sendToDMX[2]=255;
				  sendToDMX[3]=0;
				  sendToDMX[4]=0;
				  sendToDMX[5]=255;
				  ILI9341_Draw_Hollow_Rectangle_Coord(158, 58, 222, 122, WHITE);
				  xmin=158;
				  xmax=222;
				  ymin=58;
				  ymax=122;
				  HAL_TIM_Base_Start_IT(&htim17);

			  }
			  if(x_pos>160&&x_pos<220&&y_pos>140&&y_pos<200)
			  {
				  ReceiveFrom_PC();

			  }
			  if(x_pos>160&&x_pos<220&&y_pos>220&&y_pos<280)
			  {
				  ReceiveFrom_DMX();

			  }
*/
	  }
}

		  /*ILI9341_Fill_Screen(RED);
		  	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		  	  		ILI9341_Draw_Text("Dotykova obrazovka", 10, 10, BLACK, 2, WHITE);
		  	  		ILI9341_Draw_Text("Muzete kreslit", 10, 30, GREEN, 2, WHITE);
		  	  		//ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
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

		  	  						x_pos = -(position_array[1]-320);
		  	  						y_pos = position_array[0];

		  	  						ILI9341_Draw_Filled_Circle(x_pos, y_pos, 2, BLACK);

		  	  						ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		  	  						char counter_buff[30];
		  	  						sprintf(counter_buff, "POZ X: %.3d", x_pos);
		  	  						ILI9341_Draw_Text(counter_buff, 10, 80, BLACK, 2, WHITE);
		  	  						sprintf(counter_buff, "POZ Y: %.3d", y_pos);
		  	  						ILI9341_Draw_Text(counter_buff, 10, 120, BLACK, 2, WHITE);


		  	  						//ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		  	  					}



		  	          }
		  	  		}*/

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
	//---------------------------Nastavit, že tohle je jen pro příjem z UART2; pro UART1 nastavit něco jinýho a svůj program doladit dle FreeStyleru (pokud ti to nepůjde, vykašli se na to a řeš příjem)
	//Ukladani dat pro vysilani na DMX512 na zaklade toho, odkud byly prijate
	if(receiveMode==1) //Z PC do DMX
	{
		memcpy(sendToDMX, receiveBuff, 513 * sizeof(uint8_t)); //TODO: Vymyslet možnost bez kopírování (změna buffu přímo v čtecích příkazech, pointerem); bylo by využitelné pouze pro tuto funkci -> tudíž to nemá smysl (asi by to nebylo přehlednější)
		HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive);
	}
	else if(receiveMode==2)//receiveMode==2 //Z DMX do DMX
	{
		/*for(int i=0; i<513; i++) //Pro UART2 příjem (vyzkoušet pak s jinejma pultama) //zkontrolovat ten poslední byte.... //vymyslet bezposunovou variantu. - přehodit nad Receive
		{
			sendToDMX[i]=receiveBuff[i+1]; //+1 pro uart2 TODO:memcpy(&a1[50], &a2[50], 10 * sizeof a[0]);
		}*/
		memcpy(&sendToDMX[0], &receiveBuff[1], 513 * sizeof(uint8_t));
		HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive);
	}
	else if(receiveMode==3)//Z DMX do PC
	{
		memcpy(&sendToPC[2], &receiveBuff[1], 513 * sizeof(uint8_t)); //kopírování //tohle vyřešit
		HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive);
	}
	else if(receiveMode==4)//Z DMX pouze do Buffu
	{
		//memcpy(&sendToPC[2], &receiveBuff[1], 513 * sizeof(uint8_t)); //kopírování //tohle vyřešit
		HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive);
	}
	HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive);


	if(0==0) //GUI if(menuNav==5)
	{
		if(valueOffset<495)
		{

			for(int a=0;a<16;a+=3)
			{
				sprintf(str, "%d:%d  ", valueOffset+1+a, receiveBuff[valueOffset+2+a]);
				ILI9341_Draw_Text(str, 5, 20+(a*10), BLACK, 2, LIGHTGREY);
				sprintf(str, "%d:%d  ", valueOffset+2+a, receiveBuff[valueOffset+3+a]);
				ILI9341_Draw_Text(str, 120, 20+(a*10), BLACK, 2, LIGHTGREY);
				sprintf(str, "%d:%d  ", valueOffset+3+a, receiveBuff[valueOffset+4+a]);
				ILI9341_Draw_Text(str, 230, 20+(a*10), BLACK, 2, LIGHTGREY);
			}
		}
		else
		{
			for(int a=0;a<6;a+=3)
			{
				sprintf(str, "%d:%d  ", valueOffset+1+a, receiveBuff[valueOffset+2+a]);
				ILI9341_Draw_Text(str, 5, 20+(a*10), BLACK, 2, LIGHTGREY);
				sprintf(str, "%d:%d  ", valueOffset+2+a, receiveBuff[valueOffset+3+a]);
				ILI9341_Draw_Text(str, 120, 20+(a*10), BLACK, 2, LIGHTGREY);
				sprintf(str, "%d:%d  ", valueOffset+3+a, receiveBuff[valueOffset+4+a]);
				ILI9341_Draw_Text(str, 230, 20+(a*10), BLACK, 2, LIGHTGREY);
			}
			sprintf(str, "%d:%d  ", 511, receiveBuff[512]);
			ILI9341_Draw_Text(str, 5, 20+(6*10), BLACK, 2, LIGHTGREY);
			sprintf(str, "%d:%d  ", 512, receiveBuff[513]);
			ILI9341_Draw_Text(str, 120, 20+(6*10), BLACK, 2, LIGHTGREY);
			ILI9341_Draw_Text("       ", 230, 20+(6*10), BLACK, 2, LIGHTGREY);
			for(int a=9;a<16;a+=3)
			{
				ILI9341_Draw_Text("       ", 5, 20+(a*10), BLACK, 2, LIGHTGREY);
				ILI9341_Draw_Text("       ", 120, 20+(a*10), BLACK, 2, LIGHTGREY);
				ILI9341_Draw_Text("       ", 230, 20+(a*10), BLACK, 2, LIGHTGREY);
			}
		}
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) //Po odeslání -- pokud uart2 -> přepnout na IDLE; pokud uart 1 -> nic////////////////////////////////////////////////////
{
	if(huart==&huart2&&sendingToDMX==1)
	{
		if(packetsSentAfterStart<10) //ošetřuje chybu kdy po prvním startu odešle signál ve stavu tak, že ho druhá deska nenajde -> odesílá prvních 10 paketů v blokovacím režimu; netýkalo se rozpojení a zapojení signálu
		{
			SwitchPin_ToMode_GPIO_Output();
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_SET);
			uint32_t i;
			i=1000;
			while(i--); //MTBF - v blokovacím režimu
			HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive); //Aby se přijímal sinál i se spuštěním zařízení a po zapojení kabelu
			//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_RESET);
			HAL_TIM_Base_Start_IT(&htim6);
			packetsSentAfterStart++;
		}
		else //Pro všechny další chody v neblokovacím režimu
		{
			SwitchPin_ToMode_GPIO_Output();
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_SET); //Tvoří MTBF
			HAL_TIM_Base_Start_IT(&htim7);
			HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, 514); //Aby se přijímal sinál i se spuštěním zařízení a po zapojení kabelu
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) //Po doběhnutí časovače
{
	if(htim==&htim2)
	{
		memcpy(&sendToPC[2], &receiveBuff[1], 513 * sizeof(uint8_t)); //kopírování //tohle vyřešit
		HAL_UART_Transmit_IT(&huart1, sendToPC, 520); //Odeslání do PC - samostatnej timer
		HAL_TIM_Base_Start_IT(&htim2); //Spustí časovač na odesílání signálu do PC
	}
	else if(htim==&htim6)
	{
		//uint32_t i;
		HAL_TIM_Base_Stop_IT(&htim6);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_SET);
		//Zde se vytváří MAB samo
		//i=500000; //Přidat Timer na MAB, lze ji prodloužit dalším časovačem, ale přepnutí chvilku na UART chvíli trvá
		//while(i--);
		SwitchPin_ToMode_UART();
		HAL_UART_Transmit_IT(&huart2, sendToDMX, 513); //TODO: sizeof(receiveBuff)
		//receiveBuff[513]=50;

	}
	else if(htim==&htim7)
	{
		HAL_TIM_Base_Stop_IT(&htim7);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_TIM_Base_Start_IT(&htim6);
	}
}
//Lze prodloužit reset i MTBF, ale cílem je nechat to co nejkratší tak, aby se nemršil vysílanej signál -> pokud bude chodit hodně rychlej signál a do PC bude chodit s dalšíma 6 bytama navíc, dostane se ven moc pozdě - to se ale při upravování dostane tak i tak, protože ta naše potvora vysílá rychlostí ničeho... lepší by bylo odesílat jen pakety co se mají měnit; poslední možnost je úplně přepsat komunikaci s počítačem, kdy se už nebude komunikovat po DMX512 ale vlastním protokolem
//1 Dodělat analýzu
//2 Dodělat SW
//3 Build SW
//Zkusit vymyslet algoritmus pro kontrolu časování
//4 Dopsat práci
//5 Pak řešit modifikace zpráv

void ReceiveFrom_PC() {
    uartRx = &huart1;
    //receiveMode=1;
    toReceive=513;
}
void ReceiveFrom_DMX() {
    uartRx = &huart2;
    //receiveMode=2;
    toReceive=514;
}

void setTo_Analyze()
{
	ReceiveFrom_DMX(); //Nastaví příjem z DMX
	receiveMode=3; //Nastaví odesílání přijatých dat doPC
	//SwitchToReceiveOnly(); //Rozpojí relé
	HAL_TIM_Base_Start_IT(&htim2); //Spustí časovač na odesílání signálu do PC
	HAL_UART_Transmit_IT(&huart1, sendToPC, 520); //Odeslání do PC - samostatnej timer
	HAL_UARTEx_ReceiveToIdle_IT(&huart2, receiveBuff, toReceive); //Spustí přijímání z DMX
}
void setTo_GenerateOnPC()
{
	ReceiveFrom_PC(); //Nastaví příjem z PC
	SwitchToTransmit(); //Propojí relé
	sendingToDMX=1; //Povolí odesílání na DMX
	receiveMode=1; // Nastaví přijímání dat z PC a odesílání do DMX
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, receiveBuff, toReceive); //Započne přijímání z PC
	HAL_UART_Transmit_IT(&huart2, sendToDMX, 513); //Započne odesílání na DMX
}
void setTo_Modification()
{
	/*ReceiveFrom_PC(); //Nastaví příjem z PC
	SwitchToTransmit(); //Propojí relé
	sendingToDMX=1; //Povolí odesílání na DMX
	receiveMode=1; // Nastaví přijímání dat z PC a odesílání do DMX
	HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive); //Započne odesílání na DMX*/
}
void setTo_AnalyzeLocally() //Nevysílá data
{
	menuNav=5;
	ReceiveFrom_DMX(); //Nastaví příjem z DMX
	receiveMode=4;
	//SwitchToReceiveOnly(); //Rozpojí relé
	HAL_UARTEx_ReceiveToIdle_IT(&huart2, receiveBuff, 514); //Započne přijímání na DMX++
	HAL_UARTEx_ReceiveToIdle_IT(&huart2, receiveBuff, 514); //Započne přijímání na DMX
	HAL_UARTEx_ReceiveToIdle_IT(&huart2, receiveBuff, 514); //Započne přijímání na DMX
	HAL_UARTEx_ReceiveToIdle_IT(&huart2, receiveBuff, 514); //Započne přijímání na DMX
	//HAL_UARTEx_ReceiveToIdle_IT(uartRx, receiveBuff, toReceive); //Započne přijímání na DMX
}
void setTo_GenerateLocally() //Nepřijímá data
{
	SwitchToTransmit(); //Propojí relé
	sendingToDMX=1; //Povolí odesílání na DMX
	HAL_UART_Transmit_IT(&huart2, sendToDMX, 513); //Započne odesílání na DMX
}
void stopAll()
{
	HAL_TIM_Base_Stop_IT(&htim2); //Zastaví odesílání do PC
	sendingToDMX=0; //Ukončí odesílání na DMX
	receiveMode=5; //Ukončí příjem dat
	SwitchToReceiveOnly(); //Rozpojí relé
	packetsSentAfterStart=0; //Snuluje bugfix
}

void SwitchPin_ToMode_GPIO_Output(void) //TODO: přidat nastaveni PINu k prepnuti
{
    GPIO_InitTypeDef GPIO_Initialize;
    GPIO_Initialize.Pin = GPIO_PIN_2;
    GPIO_Initialize.Mode = GPIO_MODE_OUTPUT_PP;
    //GPIO_Initialize.Pull = GPIO_PULLUP;
    GPIO_Initialize.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_Initialize);
}
void SwitchPin_ToMode_UART(void)
{
    GPIO_InitTypeDef GPIO_Initialize;
    GPIO_Initialize.Pin = GPIO_PIN_2;
    GPIO_Initialize.Mode = GPIO_MODE_AF_PP;  //Alternate function
    //GPIO_Initialize.Pull = GPIO_PULLUP;
    GPIO_Initialize.Alternate = GPIO_AF7_USART2; //Musi byt pouzito pro g431
    GPIO_Initialize.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_Initialize);
}
void Draw_MainMenu(void)
{
	ILI9341_Fill_Screen(DARKGREY);
	ILI9341_Draw_Text("DMX512 ANALYZATOR", 18, 5, BLACK, 2, DARKGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(10, 24, 230, 64, LIGHTGREY);
	ILI9341_Draw_Text("Analyza signalu", 27, 34, BLACK, 2, LIGHTGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(10, 70, 230, 110, LIGHTGREY);
	ILI9341_Draw_Text("Generovani signalu", 12, 80, BLACK, 2, LIGHTGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(10, 116, 230, 156, LIGHTGREY);
	ILI9341_Draw_Text("Modifikace signalu", 12, 126, BLACK, 2, LIGHTGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(10, 162, 230, 202, LIGHTGREY);
	ILI9341_Draw_Text("Manualni analyza", 22, 172, BLACK, 2, LIGHTGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(10, 208, 230, 248, LIGHTGREY);
	ILI9341_Draw_Text("Manualni gen.", 38, 218, BLACK, 2, LIGHTGREY);
}
void Draw_MainMenu1_Horizontal(void)
{
	menuNav=1;
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Fill_Screen(DARKGREY);
	ILI9341_Draw_Text("DMX512 ANALYZATOR", 10, 5, BLACK, 2, DARKGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(20, 29, 300, 79, LIGHTGREY); //+10
	ILI9341_Draw_Text("Analyza signalu", 68, 44, BLACK, 2, LIGHTGREY); //+5
	ILI9341_Draw_Filled_Rectangle_Coord(20, 85, 300, 135, LIGHTGREY);
	ILI9341_Draw_Text("Generovani signalu", 48, 100, BLACK, 2, LIGHTGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(20, 141, 300, 191, LIGHTGREY);
	ILI9341_Draw_Text("Modifikace signalu", 48, 156, BLACK, 2, LIGHTGREY);
	ILI9341_Draw_Text("1/2", 145, 220, BLACK, 2, DARKGREY);
	Draw_NavigationBar_Right_Horizontal();
}
void Draw_MainMenu2_Horizontal(void)
{
	menuNav=2;
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Fill_Screen(DARKGREY);
	ILI9341_Draw_Text("DMX512 ANALYZATOR", 10, 5, BLACK, 2, DARKGREY);
	ILI9341_Draw_Filled_Rectangle_Coord(20, 29, 300, 79, LIGHTGREY); //+10
	ILI9341_Draw_Text("Manualni analyza", 68, 44, BLACK, 2, LIGHTGREY); //+5
	ILI9341_Draw_Filled_Rectangle_Coord(20, 85, 300, 135, LIGHTGREY);
	ILI9341_Draw_Text("Manualni generovani", 45, 100, BLACK, 2, LIGHTGREY);
	ILI9341_Draw_Text("2/2", 145, 220, BLACK, 2, DARKGREY);
	Draw_NavigationBar_Left_Horizontal();
}
void Draw_NavigationBar_Left(void)
{
	ILI9341_Draw_Filled_Rectangle_Coord(0, 280, 50, 320, LIGHTGREY);
	ILI9341_Draw_Text("<-", 20, 290, BLACK, 2, DARKGREY);
}
void Draw_NavigationBar_Right(void)
{
	ILI9341_Draw_Filled_Rectangle_Coord(190, 280, 240, 320, LIGHTGREY);
	ILI9341_Draw_Text("->",210 , 290, BLACK, 2, DARKGREY);
}
void Draw_NavigationBar_Left_Horizontal(void)
{
	ILI9341_Draw_Filled_Rectangle_Coord(0, 205, 70, 240, LIGHTGREY);
	ILI9341_Draw_Text("<-", 25, 214, BLACK, 2, DARKGREY);
}
void Draw_NavigationBar_Right_Horizontal(void)
{
	ILI9341_Draw_Filled_Rectangle_Coord(250, 205, 320, 240, LIGHTGREY);
	ILI9341_Draw_Text("->",275 , 214, BLACK, 2, DARKGREY);
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
