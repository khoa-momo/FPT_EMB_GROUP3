#include"READFAT.h"


/* This funsstion get data of boot sector */

void GetBoostector(FILE *fptr)
{
    SET_FILE_POITER(BYTE_OF_SECTOR);
    for(int i = 0; i < 2; i++)
    {
        BootSector.byte_of_sector += (fgetc(fptr) << (8 * i));
    }

    BootSector.sector_of_cluster += fgetc(fptr);

    for(int i = 0; i < 2; i++)
    {
        BootSector.sector_before_fat += (fgetc(fptr) << (8 * i));
    }

    BootSector.count_of_fat += fgetc(fptr);

    for(int i = 0; i < 2; i++)
    {
        BootSector.count_entry_of_rdet += (fgetc(fptr) << (8 * i));
    }

    SET_FILE_POITER(SECTOR_OF_FAT);
    for(int i = 0; i < 2; i++)
    {
        BootSector.sector_of_fat += (fgetc(fptr) << (8 * i));
    }

    SET_FILE_POITER(SECTOR_OF_VOLUME);
        for(int i = 0; i < 2; i++)
    {
        BootSector.sector_of_volume += (fgetc(fptr) << (8 * i));
    }

    if(BootSector.sector_of_volume == 0)
    {
        SET_FILE_POITER(0x19);
        for(int i = 0; i < 4; i++)
        {
            BootSector.sector_of_volume += (fgetc(fptr) << (8 * i));
        }
    }

}





