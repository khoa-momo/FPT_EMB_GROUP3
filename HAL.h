#ifndef ___HAL_H___
#define ___HAL_H___

#include"FAT_system.h"

typedef enum FAT{
	FAT12=1,
	FAT16=2,
	FAT32=3
}FAT;

void callBootSector();
FAT checkFAT();

uint16_t GetFatValue( uint16_t PrsClus);
int getDataFile(Entry_Short *entr_sh, int index);

void readDataNode();
void checkFile(Entry_Short *entr_sh,int cnt_entr_sh);
void callData();

#endif
