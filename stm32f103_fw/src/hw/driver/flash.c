/*
 * flash.c
 *
 *  Created on: Aug 25, 2024
 *      Author: jih
 */


#include "flash.h"


#ifdef _USE_HW_FLASH

#define FLASH_SECTOR_MAX    8

typedef struct
{
  uint32_t addr;
  uint32_t length;
} flash_tbl_t;


//const flash_tbl_t flash_tbl[FLASH_SECTOR_MAX]={
//    {FLASH_SECTOR_0, 16 * 1024},
//    {FLASH_SECTOR_1, 16 * 1024},
//    {FLASH_SECTOR_2, 16 * 1024},
//    {FLASH_SECTOR_3, 16 * 1024},
//    {FLASH_SECTOR_4, 64 * 1024},
//    {FLASH_SECTOR_5, 128 * 1024},
//    {FLASH_SECTOR_6, 128 * 1024},
//    {FLASH_SECTOR_7, 128 * 1024}
//};

const flash_tbl_t flash_tbl[FLASH_SECTOR_MAX]={
    {0x08000000, 16 * 1024},
    {0x08004000, 16 * 1024},
    {0x08008000, 16 * 1024},
    {0x0800C000, 16 * 1024},
    {0x08010000, 64 * 1024},
    {0x08020000, 128 * 1024},
    {0x08040000, 128 * 1024},
    {0x08060000, 128 * 1024}
};


static bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length);



bool flashInit(void)
{
  // F103에서 flash_tbl[] 초기
//  for (int i = 0; i < FLASH_SECTOR_MAX; ++i)
//  {
//    flash_tbl[i].addr   = 0x80000000 + i*1024;
//    flash_tbl[i].length = 1024;
//  }

  return true;
}

bool flashErase(uint32_t addr, uint32_t length)
{
  bool ret = false;
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef init;
  uint32_t sector_error;

  int16_t start_sector_num = -1;
  uint32_t sector_count = 0;

  for (int i = 0; i < FLASH_SECTOR_MAX; ++i) {
    if (flashInSector(i, addr, length) == true)
    {
      if (start_sector_num < 0)
      {
        start_sector_num = i;
      }
      sector_count++;
    }
  }

  if (sector_count > 0)
  {
    HAL_FLASH_Unlock();

    init.TypeErase    = FLASH_TYPEERASE_SECTORS;
    init.Banks        = FLASH_BANK_1;
    init.Sector       = start_sector_num;
    init.NbSectors    = sector_count;
    init.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    status = HAL_FLASHEx_Erase(&init, &sector_error);
    if (status == HAL_OK)
    {
      ret = true;
    }

    HAL_FLASH_Lock();
  }

  return ret;
}

bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  HAL_StatusTypeDef status;

  if (addr %2 != 0)
  {
    return false;
  }

  HAL_FLASH_Unlock();

  for (int i = 0; i < length; ++i) {
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr + i, (uint64_t)p_data[i]);
    if (status != HAL_OK)
    {
      ret = false;
      break;
    }
  }

  HAL_FLASH_Lock();

  return ret;
}

bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint8_t *p_byte = (uint8_t *)addr;

  for (int i = 0; i < length; ++i) {
    p_data[i] = p_byte[i];
  }

  return ret;
}


bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length)
{
  bool ret = false;

  uint32_t sector_start;
  uint32_t sector_end;
  uint32_t flash_start;
  uint32_t flash_end;

  sector_start = flash_tbl[sector_num].addr;
  sector_end   = sector_start + flash_tbl[sector_num].length - 1;
  flash_start  = addr;
  flash_end    = flash_start + length - 1;

  if (sector_start >= flash_start && sector_start <= flash_end)
  {
    ret = true;
  }

  if (sector_end >= flash_start && sector_end <= flash_end)
  {
    ret = true;
  }

  if (flash_start >= sector_start && flash_start <= sector_end)
  {
    ret = true;
  }

  if (flash_end >= sector_start && flash_end <= sector_end)
  {
    ret = true;
  }

  return ret;
}

#endif
