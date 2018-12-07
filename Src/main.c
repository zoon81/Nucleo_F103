
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "rgb.h"
#include "keyboard_matrix.h"
#include "itoa.h"
#include "lcd.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

struct buffer_s
{
  char *buffer;
  uint16_t tmpbuff;
  uint8_t index;
  uint8_t size;
  uint8_t rxdone;
};
struct buffer_s uartRxBuffer;
uint16_t adc_raw;
uint16_t Vpot;
uint8_t dma_done;

struct rgb_t rgbValue;
const char getR[] = "\n\rRED:";
const char getG[] = "\n\rGREEN:";
const char getB[] = "\n\rBLUE:";

uint8_t refresh_display;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void display_rgb_values();
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *adchandle){
//     adc_raw = adchandle->Instance->DR;
//     Vpot = (3300 * adc_raw) / 4095;
// }
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (uartRxBuffer.tmpbuff == 13)
  {
    // Terminate buffer
    uartRxBuffer.buffer[uartRxBuffer.index] = 0;
    uartRxBuffer.rxdone = 1;
  }
  else
  {
    if (uartRxBuffer.tmpbuff < 58 && uartRxBuffer.tmpbuff > 47)
    {
      if (uartRxBuffer.index < uartRxBuffer.size)
      {
        uartRxBuffer.buffer[uartRxBuffer.index] = (uint8_t)uartRxBuffer.tmpbuff;
        HAL_UART_Transmit(&huart2, &(uartRxBuffer.buffer[uartRxBuffer.index]), 1, 10);
        uartRxBuffer.index++;
      }
    }
    HAL_UART_Receive_IT(&huart2, &uartRxBuffer.tmpbuff, 1);
  }
}

void adcdmaCallBack(DMA_HandleTypeDef dma)
{
  dma_done = 1;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  //Init uartRX buffer
  uint8_t rxBuffer[4];
  uartRxBuffer.buffer = &rxBuffer[0];
  uartRxBuffer.index = 0;
  uartRxBuffer.size = sizeof(rxBuffer);
  uartRxBuffer.rxdone = 0;

  // Struct hold pwm values for driving an rgb led
  rgbValue.red = 0;
  rgbValue.green = 0;
  rgbValue.blue = 0;
  rgbValue.index = 0;

  refresh_display = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  rgb_init_start(&htim4);
  kbm_init();
  LCD_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart2, getR, sizeof(getR), 500);
  LCD_String_xy(0, 0, "RGB LED VALUES");
  LCD_String_xy(1, 0, "RED: ");
  LCD_String_xy(2, 0, "GREEN: ");
  LCD_String_xy(3, 0, "BLUE: ");
  display_rgb_values();
  //HAL_ADC_Start_IT(&hadc1);
  uint32_t dma_buffer[3];
  //HAL_DMA_RegisterCallback(&hdma_adc1, HAL_DMA_XFER_CPLT_CB_ID, adcdmaCallBack);
  HAL_ADC_Start_DMA(&hadc1, dma_buffer, 3);
  uint16_t ch8, temp, Vin;
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  date.WeekDay = RTC_WEEKDAY_SATURDAY;
  date.Year = 18;
  date.Month = 12;
  date.Date = 2;
  time.Hours = 11;
  time.Minutes = 41;
  time.Seconds = 0;
  HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
  HAL_UART_Receive_IT(&huart2, &(uartRxBuffer.tmpbuff), 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //RTC READ
    //HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    //HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    // HAL_Delay(1000);
    if (uartRxBuffer.rxdone != 0)
    {
      switch (rgbValue.index)
      {
      case 0:
      {
        if (uartRxBuffer.buffer[0] != 0)
        {
          uint8_t tmp = atoi_(uartRxBuffer.buffer);
          tmp = tmp % 100;
          rgbValue.red = tmp;
          rgb_set(&htim4, &rgbValue);
        }
        HAL_UART_Transmit(&huart2, getG, sizeof(getG), 500);
        rgbValue.index++;
        uartRxBuffer.rxdone = 0;
        uartRxBuffer.index = 0;
        HAL_UART_Receive_IT(&huart2, &(uartRxBuffer.tmpbuff), 1);
        break;
      }
      case 1:
      {
        if (uartRxBuffer.buffer[0] != 0)
        {
          uint8_t tmp = atoi_(uartRxBuffer.buffer);
          tmp = tmp % 100;
          rgbValue.green = tmp;
          rgb_set(&htim4, &rgbValue);
        }
        HAL_UART_Transmit(&huart2, getB, sizeof(getB), 500);
        rgbValue.index++;
        uartRxBuffer.rxdone = 0;
        uartRxBuffer.index = 0;
        HAL_UART_Receive_IT(&huart2, &(uartRxBuffer.tmpbuff), 1);
        break;
      }
      case 2:
      {
        if (uartRxBuffer.buffer[0] != 0)
        {
          uint8_t tmp = atoi_(uartRxBuffer.buffer);
          tmp = tmp % 100;
          rgbValue.blue = tmp;
          rgb_set(&htim4, &rgbValue);
        }
        HAL_UART_Transmit(&huart2, getR, sizeof(getR), 500);
        rgbValue.index = 0;
        uartRxBuffer.rxdone = 0;
        uartRxBuffer.index = 0;
        HAL_UART_Receive_IT(&huart2, &(uartRxBuffer.tmpbuff), 1);
        break;
      }
      default:
      {
        break;
      }
      }
      refresh_display = 1;
    }
    // KBM
    if (!(HAL_GetTick() % 100))
    {
      uint8_t press_key = kbm_getValue((uint8_t)0);
      if (press_key != 0xFF)
      {
        switch (press_key)
        {
        case 1:
          rgbValue.red--;
          rgb_set(&htim4, &rgbValue);
          break;
        case 7:
          rgbValue.red++;
          rgb_set(&htim4, &rgbValue);
          break;
        case 2:
          rgbValue.green--;
          rgb_set(&htim4, &rgbValue);
          break;
        case 8:
          rgbValue.green++;
          rgb_set(&htim4, &rgbValue);
          break;
        case 3:
          rgbValue.blue--;
          rgb_set(&htim4, &rgbValue);
          break;
        case 9:
          rgbValue.blue++;
          rgb_set(&htim4, &rgbValue);
          break;

        default:
          break;
        }
        refresh_display = 1;
      }
    }
    if (refresh_display)
    {
      display_rgb_values();
      refresh_display = 0;
    }
    //ADC Polling mode
    // HAL_ADC_Start(&hadc1);
    // HAL_ADC_PollForConversion(&hadc1, 100);
    // temp = HAL_ADC_GetValue(&hadc1);
    // HAL_Delay(100);
    // HAL_ADC_PollForConversion(&hadc1, 100);
    // Vin = HAL_ADC_GetValue(&hadc1);
    // HAL_Delay(100);
    // HAL_ADC_PollForConversion(&hadc1, 100);
    // ch8 = HAL_ADC_GetValue(&hadc1);
    // HAL_ADC_Stop(&hadc1);
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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void display_rgb_values()
{
  char red_str[4];
  char green_str[4];
  char blue_str[4];
  uint8_to_str(red_str, rgbValue.red);
  uint8_to_str(green_str, rgbValue.green);
  uint8_to_str(blue_str, rgbValue.blue);
  LCD_Clear_xy(1, 5, 3);
  LCD_String_xy(1, 5, red_str);
  //LCD_String(red_str);
  LCD_Clear_xy(2, 6, 3);
  LCD_String_xy(2, 6, green_str);
  //LCD_String(green_str);
  LCD_Clear_xy(3, 5, 3);
  LCD_String_xy(3, 5, blue_str);
  //LCD_String(blue_str);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
