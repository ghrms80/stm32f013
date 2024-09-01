/*
 * reset.c
 *
 *  Created on: Aug 17, 2024
 *      Author: jih
 */


#include "reset.h"
#include "rtc.h"

static uint32_t reset_count = 0;


bool resetInit(void)
{
  bool ret = true;

  if (RCC->CSR == (1<<26))
  {
    rtcBackupRegWrite(0, rtcBackupRegRead(0) + 1); // 자신을 1 증가
    delay(500);  // 0.5 초안에 누르지 않으면 카운트 종료
    reset_count = rtcBackupRegRead(0); // 누른 횟수 저장
  }

  rtcBackupRegWrite(0, 0); // 다시 0으로 초기화

  if (reset_count != 2)
  {
    void (**jump_func)() = (void (**)())(0x08008000 + 4);

    if ((uint32_t)(*jump_func) != 0xFFFFFFFF)
    {
      HAL_RCC_DeInit();
      HAL_DeInit();

      for (int i = 0; i < 8; ++i) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        __DSB();
        __ISB();
      }
      SysTick->CTRL = 0;

      (*jump_func)();
    }
  }

  return ret;
}

uint32_t resetGetCount(void)
{
  return reset_count;
}
