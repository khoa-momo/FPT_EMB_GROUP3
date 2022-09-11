#ifndef _READ_FAT_
#define _READ_FAT_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/* STRUCT OF BOOT SECTOR */
#define BYTE_OF_SECTOR                  0X0B
#define SECTOR_OF_CLUSTER               0x0D
#define SECTOR_BEFORE_FAT               0X0E
#define COUNT_OF_FAT                    0X10
#define COUNT_ENTRY_OF_RDET             0X11
#define SECTOR_OF_VOLUME                0X13 /* = 0 IF > 65535 (GET VALUE OF SECTOR_OF_VOLUME2)*/
#define SECTOR_OF_FAT                   0X16 /* ONE FAT */

typedef struct
{
    uint16_t byte_of_sector;
    uint8_t sector_of_cluster;
    uint16_t sector_before_fat;
    uint8_t count_of_fat;
    uint16_t count_entry_of_rdet;
    uint16_t sector_of_volume;
    uint16_t sector_of_fat;

}BOOT_SECTOR;
BOOT_SECTOR BootSector;

/* FIRST ADDRESS OF FAT TABLE */
#define FIRST_ADDRESS_OF_FAT_TABLE

/* FIRST ADDRESS OF ROOT */
#define FIRST_ADDRESS_OF_ROOT             (BootSector.sector_of_fat * BootSector.count_of_fat * BootSector.byte_of_sector) + (BootSector.sector_before_fat * BootSector.byte_of_sector)

/* STRUCT OF ROOT DIRECTORY MAIN ENTRY TABLE */
#define FIRST_CLUSTER                     0X1A



/* CHECK MAIN ENTRY OR SUB ENTRY */
#define CHECK_ENTRY                        0X0B



/* SET FILE POITER */

#define SET_FILE_POITER(Address)              fseek(fptr , Address , SEEK_SET)

//void CheckEntry(fptr)
uint16_t GetFatValue12(FILE *fptr, uint16_t PrsClus); /*for FAT12*/
uint16_t GetFatValue16(FILE *fptr, uint16_t PrsClus); /*for FAT16*/
uint32_t GetFatValue32(FILE *fptr, uint32_t PrsClus); /*for FAT16*/
void ReadRoot (FILE *fptr);
//uint8_t check_entry;
#endif // _READ_FAT_
