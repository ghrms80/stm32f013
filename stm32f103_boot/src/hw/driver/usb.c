/*
 * usb.c
 *
 *  Created on: Aug 16, 2024
 *      Author: jih
 */


#include "usb.h"
#include "usb_device.h"

bool usbInit(void)
{
  bool ret = true;

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12; // DP(D+)
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // Open Drain
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 100ms 정도 low로 내렸다가 다시 올림
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
  delay(100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);

  // PA12 핀을 input으로 재설정
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  MX_USB_DEVICE_Init();

  return ret;
}
