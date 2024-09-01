/*
 * bsp.c
 *
 *  Created on: Aug 12, 2024
 *      Author: jih
 */


#include "bsp.h"
#include "uart.h"


void SystemClock_Config(void);


void bspInit(void)
{
  HAL_Init();
  SystemClock_Config();

  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();


  ///////////// usb pin을 먼저 LOW로 설정 (reset 상태) /////////////
//  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//
//  /*Configure GPIO pin : PA5 */
//  GPIO_InitStruct.Pin = GPIO_PIN_12; // DP
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // Drain
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
//  // 200ms 정도 low로 내렸다가 다시 올림
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
  /////////////////////////////////////////////////////////////
}

void delay(uint32_t ms)
{
#ifdef _USE_HW_RTOS
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    osDelay(ms);    // 스레드가 suspend 되어 wait를 수행
  }                 // 여러 스레드에서 실행되어도 성능에 영향을 미치지 않음
  else
  {
    HAL_Delay(ms);  // busy-wait : cpu 리소스 점유
  }                 // 여러 스레드에서 이 함수를 사용하면 성능이 저하됨
#else
  HAL_Delay(ms);
#endif
}

uint32_t millis(void)
{
  return HAL_GetTick();
}


int __io_putchar(int ch)
{
  uartWrite(_DEF_UART2, (uint8_t *)&ch, 1);

  return 1;
}


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
