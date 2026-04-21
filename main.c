/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void LCD_EnablePulse(void);
void LCD_Send4Bit(uint8_t data);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Init(void);
void LCD_SendString(char *str);

void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Clear(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Keypad functions */
char Keypad_Scan(void);

/* Key map */
char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

uint16_t Read_ADC()
{
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  return HAL_ADC_GetValue(&hadc1);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint16_t value;
	char buffer[16];
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(50);
	LCD_Init();


//	LCD_SendString("Hello");
//	LCD_SendCommand(0xC0);
//	LCD_SendString("Water Sensor");


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  value = Read_ADC();


	  LCD_Clear();
	  LCD_SetCursor(0, 0);
	  LCD_SendString("Water Sensor");

	  LCD_SetCursor(1, 0);

	  int percent = (value * 100) / 4095;

	  sprintf(buffer, "Water: %d%%", percent);
	  LCD_SendString(buffer);

	  // Hiển thị giá trị số
//	  sprintf(buffer, "Val: %d", value);
//
//	  LCD_SendString(buffer);


	  if (value < 1000)
	  {
		LCD_SendString(" Dry");
	  }
	  else if (value < 2000)
	  {
		LCD_SendString(" Moist");
	  }
	  else
	  {
		LCD_SendString(" Water!!!");
	  }

	  HAL_Delay(1000);

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_12|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB12 PB4 PB5 PB6
                           PB7 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_12|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB13 PB14 PB15 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// Ghi mức logic ra các chân D4-D7
void LCD_Send4Bit(uint8_t data)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);   // D4 -> PB2
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);   // D5 -> PB10
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);   // D6 -> PB4
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);   // D7 -> PB12
}

// Tạo xung Enable
void LCD_EnablePulse(void)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);   // E = 1
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // E = 0
  HAL_Delay(1);
}

// Gửi lệnh cho LCD
void LCD_SendCommand(uint8_t cmd)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // RS = 0 => command

  LCD_Send4Bit(cmd >> 4);      // gửi 4 bit cao
  LCD_EnablePulse();

  LCD_Send4Bit(cmd & 0x0F);    // gửi 4 bit thấp
  LCD_EnablePulse();

  HAL_Delay(2);
}

// Gửi dữ liệu cho LCD
void LCD_SendData(uint8_t data)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // RS = 1 => data

  LCD_Send4Bit(data >> 4);     // gửi 4 bit cao
  LCD_EnablePulse();

  LCD_Send4Bit(data & 0x0F);   // gửi 4 bit thấp
  LCD_EnablePulse();

  HAL_Delay(2);
}

// Khởi tạo LCD 1602A chế độ 4-bit
void LCD_Init(void)
{
  HAL_Delay(40);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // RS = 0

  // quy trình init chuẩn cho 4-bit
  LCD_Send4Bit(0x03);
  LCD_EnablePulse();
  HAL_Delay(5);

  LCD_Send4Bit(0x03);
  LCD_EnablePulse();
  HAL_Delay(1);

  LCD_Send4Bit(0x03);
  LCD_EnablePulse();
  HAL_Delay(1);

  LCD_Send4Bit(0x02); // chuyển sang 4-bit mode
  LCD_EnablePulse();
  HAL_Delay(1);

  LCD_SendCommand(0x28); // 4-bit, 2 dòng, font 5x8
  LCD_SendCommand(0x0C); // bật màn hình, tắt con trỏ
  LCD_SendCommand(0x06); // tự động tăng địa chỉ
  LCD_SendCommand(0x01); // xóa màn hình
  HAL_Delay(2);
}

// Gửi chuỗi ký tự
void LCD_SendString(char *str)
{
  while (*str)
  {
    LCD_SendData((uint8_t)(*str));
    str++;
  }
}

//4x4 Matrix
void LCD_SetCursor(uint8_t row, uint8_t col)
{
  uint8_t address;

  if (row == 0)
    address = 0x80 + col;
  else
    address = 0xC0 + col;

  LCD_SendCommand(address);
}

void LCD_Clear(void)
{
  LCD_SendCommand(0x01);
  HAL_Delay(2);
}

char Keypad_Scan(void)
{
  GPIO_TypeDef* rowPorts[4] = {GPIOB, GPIOB, GPIOB, GPIOB};
  uint16_t rowPins[4] = {GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8};

  GPIO_TypeDef* colPorts[4] = {GPIOB, GPIOB, GPIOB, GPIOB};
  uint16_t colPins[4] = {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_3};

  int row, col;

  // Đặt tất cả row = HIGH
  for (row = 0; row < 4; row++)
  {
    HAL_GPIO_WritePin(rowPorts[row], rowPins[row], GPIO_PIN_SET);
  }

  for (row = 0; row < 4; row++)
  {
    // kéo row hiện tại xuống LOW
    HAL_GPIO_WritePin(rowPorts[row], rowPins[row], GPIO_PIN_RESET);

    for (col = 0; col < 4; col++)
    {
      if (HAL_GPIO_ReadPin(colPorts[col], colPins[col]) == GPIO_PIN_RESET)
      {
        HAL_Delay(20); // debounce

        if (HAL_GPIO_ReadPin(colPorts[col], colPins[col]) == GPIO_PIN_RESET)
        {
          // trả row về HIGH trước khi return
          HAL_GPIO_WritePin(rowPorts[row], rowPins[row], GPIO_PIN_SET);
          return keys[row][col];
        }
      }
    }

    // trả row hiện tại về HIGH
    HAL_GPIO_WritePin(rowPorts[row], rowPins[row], GPIO_PIN_SET);
  }

  return 0;
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
