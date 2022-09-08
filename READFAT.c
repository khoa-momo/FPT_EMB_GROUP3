#include "READFAT.h"

/* This funstion get value of FAT entry*/

uint16_t GetFatValue12(FILE *fptr, uint16_t PrsClus) /*for FAT12*/{
    unsigned char DataOfIndex[2]; /* array save 2 hex of FAT*/
    uint16_t NewVal;
    uint16_t HIG;
    uint16_t LOW;

    fseek (fptr , 512 + (PrsClus * 3) / 2 , SEEK_SET);

    /* get data into array */
    DataOfIndex[0] = fgetc(fptr);
    DataOfIndex[1] = fgetc(fptr);
    HIG = DataOfIndex[1];
    LOW = DataOfIndex[0];

    if (PrsClus % 2 == 0)
    {
        NewVal = (LOW | ((HIG & 0xF) << 8));
    }
    else
    {
        NewVal = ((HIG << 4) | (LOW >> 4));
    }
}

uint16_t GetFatValue16(FILE *fptr, uint16_t PrsClus) /*for FAT16*/{
    unsigned char DataOfIndex[2]; /* array save 2 hex of FAT*/
    uint16_t NewVal;
    uint16_t HIG;
    uint16_t LOW;

    fseek (fptr , 512 + (PrsClus * 2) , SEEK_SET);

     /* get data into array */
    DataOfIndex[0] = fgetc(fptr);
    DataOfIndex[1] = fgetc(fptr);

    HIG = DataOfIndex[1];
    LOW = DataOfIndex[0];

    NewVal = (HIG << 8) | LOW;
    return NewVal;
}

uint32_t GetFatValue32(FILE *fptr, uint32_t PrsClus) /*for FAT16*/{
    uint32_t DataOfIndex[4]; /* array save 4 hex of FAT*/
    uint32_t NewVal;
    uint32_t HIG;
    uint32_t LOW;
    uint32_t HIGEST;
    uint32_t LOWEST;

    fseek (fptr , 512 + (PrsClus * 4) , SEEK_SET);

     /* get data into array */
    DataOfIndex[0] = fgetc(fptr);
    DataOfIndex[1] = fgetc(fptr);
    DataOfIndex[2] = fgetc(fptr);
    DataOfIndex[3] = fgetc(fptr);

    HIGEST = DataOfIndex[3];
    HIG = DataOfIndex[2];
    LOW = DataOfIndex[1];
    LOWEST = DataOfIndex[0];

    NewVal = (((HIGEST << 18) | (HIG << 10)) | (LOW << 8)) | (LOWEST);

    return NewVal;
}
