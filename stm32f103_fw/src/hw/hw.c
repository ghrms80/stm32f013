/*
 * hw.c
 *
 *  Created on: Aug 12, 2024
 *      Author: jih
 */

#include "hw.h"

void hwInit(void)
{
  bspInit();
  rtcInit();

  cliInit();
  resetInit();
  ledInit();
  usbInit();
  uartInit();
  flashInit();
}
