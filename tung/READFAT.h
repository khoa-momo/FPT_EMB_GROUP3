#ifndef _READ_FAT_
#define _READ_FAT_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint16_t GetFatValue12(FILE *fptr, uint16_t PrsClus);
uint16_t GetFatValue16(FILE *fptr, uint16_t PrsClus);
uint32_t GetFatValue32(FILE *fptr, uint32_t PrsClus);

#endif // _READ_FAT_


