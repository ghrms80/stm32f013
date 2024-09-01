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

  resetInit();
  ledInit();
  usbInit();
  flashInit();
}
