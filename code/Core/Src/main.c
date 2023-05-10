/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
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
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
uint8_t RxBuffer[2];
uint8_t TxBuffer[200] = {0};

uint8_t MyHz[5];

uint8_t Input = 0;
uint8_t OnOff = 0;
int8_t Hz = 5;
uint8_t Mode0Flag = 0;
uint8_t Mode1Flag = 0;

uint8_t back[] =
		"\r\nBack to main menu\r\n\r\nPlease select mode\r\n -> 0 : LED Control\r\n -> 1 : Button Status\r\n__________________________\r\n";
uint8_t unpress[] = "Unpress Button\r\n";
uint8_t press[] = "Press Button\r\n";
uint8_t welcome[] =
		"__________________________\r\n\r\n   Welcome to main menu!  \r\n__________________________\r\n\r\n -> 0 : LED Control\r\n -> 1 : Button Status\r\n";

// create structure type
struct _GPIOState {
	GPIO_PinState Current;
	GPIO_PinState Last;
};

// declare variable
struct _GPIOState Button1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void UARTInterruptConfig();
void AssignMode();
void ButtonStatus();
void LEDControl();
void BlinkLED();
void WrongInput();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

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
	MX_DMA_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Transmit_DMA(&huart2, welcome, sizeof(welcome) - 1);

	UARTInterruptConfig();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);

		if (Mode1Flag) {
			ButtonStatus();
		}
		if (OnOff == 1) {
			BlinkLED();
		} else if (OnOff == 0) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 57600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	/* DMA1_Stream6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PA0 LD2_Pin */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// Blink LED x Hz
void BlinkLED() {
	static uint32_t timestamp = 0;
	if (HAL_GetTick() >= timestamp) {
		timestamp = HAL_GetTick() + (Hz * 500);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}

void UARTInterruptConfig() {
	// start UART in DMA mode
	HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart2) {
		Input = RxBuffer[0];
		AssignMode();

		// recall receive interrupt
		HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
	}
}
void AssignMode() {
	if (Input == 120) {
		Mode0Flag = 0;
		Mode1Flag = 0;
		sprintf((char*) TxBuffer, back);
		HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
	}
	// mode 0
	else if ((Input == 48) || (Mode0Flag == 1)) { // 97 100 115
		if (Mode0Flag == 0) {
			Mode0Flag = 1;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
			sprintf((char*) TxBuffer,"\r\nYour input : %c \r\nYou are in mode LED Control\r\n - a : Speed Up + 1 Hz\r\n - s : Speed Down - 1 Hz\r\n - d : On/Off\r\n - x : Back to main menu\r\n__________________________\r\n", RxBuffer[0]);
			HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
		} else {
			LEDControl();
		}
	}
	// mode 1
	else if (Input == 49){
		if (Mode1Flag == 0) {
			Mode1Flag = 1;
			Button1.Last = 1;
			sprintf((char*) TxBuffer,
					"\r\nYour input : %c \r\nYou are in mode Button Status\r\n -> Press blue button on nucleo board\r\n -> x : Back to main menu\r\n__________________________\r\n", RxBuffer[0]);
			HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
		}
	}
	else {
		WrongInput();
	}
}

// mode buttonStatus
void ButtonStatus() {
	Button1.Current = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	// detect button press by using failing edge detector
	if (Button1.Last == 0 && Button1.Current == 1) {
		sprintf((char*) TxBuffer, unpress);
		HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
	} else if (Button1.Last == 1 && Button1.Current == 0) {
		sprintf((char*) TxBuffer, press);
		HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
	}

	// update
	Button1.Last = Button1.Current;
}

void LEDControl() {
	switch (Input) {
	default:
		WrongInput();
		break;

	case 97: // speed up + 1 Hz
		Hz = Hz + 1;
		sprintf((char*) TxBuffer,
				"\r\nSpeed Up 1 Hz\r\nLED blink(Hz): %d\r\n\r\nMenu LED Control\r\n - a : Speed Up + 1 Hz\r\n - s : Speed Down - 1 Hz\r\n - d : On/Off\r\n - x : Back to main menu\r\n__________________________\r\n", Hz);
		HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
		break;

	case 100: // On / Off
		if (OnOff == 1) {
			OnOff = 0;
			sprintf((char*) TxBuffer, "\r\noff LED\r\n\r\nMenu LED Control\r\n - a : Speed Up + 1 Hz\r\n - s : Speed Down - 1 Hz\r\n - d : On/Off\r\n - x : Back to main menu\r\n__________________________\r\n");
			HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
		} else if (OnOff == 0) {
			OnOff = 1;
			sprintf((char*) TxBuffer, "\r\non LED\r\nLED blink(Hz): %d\r\n\r\nMenu LED Control\r\n - a : Speed Up + 1 Hz\r\n - s : Speed Down - 1 Hz\r\n - d : On/Off\r\n - x : Back to main menu\r\n__________________________\r\n",
					Hz);
			HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
		}
		break;

	case 115: // speed down - 1 Hz
		Hz = Hz - 1;
		if(Hz < 0){
			Hz = Hz + 1;
			sprintf((char*) TxBuffer,
					"\r\nLED does not blink\r\n\r\nMenu LED Control\r\n - a : Speed Up + 1 Hz\r\n - s : Speed Down - 1 Hz\r\n - d : On/Off\r\n - x : Back to main menu\r\n__________________________\r\n");
			HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));
		}
		else if(Hz >= 0){
			sprintf((char*) TxBuffer,
					"\r\nSpeed Down 1 Hz\r\nLED blink(Hz): %d\r\n\r\nMenu LED Control\r\n - a : Speed Up + 1 Hz\r\n - s : Speed Down - 1 Hz\r\n - d : On/Off\r\n - x : Back to main menu\r\n__________________________\r\n", Hz);
			HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) TxBuffer));}
		break;
	}
}

void WrongInput() {
	sprintf((char*) TxBuffer,
			"\r\nYour input : %c \r\nIncorrect input try again\r\nPress x : Back to main menu\r\n__________________________\r\n", RxBuffer[0]);
	HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*) (TxBuffer)));
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
