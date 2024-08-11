/*
 * main.c
 *
 *  Created on: Aug 12, 2024
 *      Author: jih
 */


#include "main.h"


int main(void)
{
  hwInit();
  apInit();

  apMain();

  return 0;
}
