/*
 * ap.c
 *
 *  Created on: Aug 12, 2024
 *      Author: jih
 */

#include "ap.h"



void apInit(void)
{

}

void apMain(void)
{
  while(1)
  {
    ledToggle(_DEF_LED1);
    delay(500);
  }
}
