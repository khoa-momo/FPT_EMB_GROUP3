#include "FAT.h"
///////Read N Byte From Offset
int ReadnByte(int n,int offset,FILE *fp){  
    int *buff = (int *)malloc(n*sizeof(int));
    fseek(fp,offset,SEEK_SET);
    for(int i=0;i<n;i++)
    {
        buff[i]=fgetc(fp);
    }
    int c=buff[0];
    for(int i=0;i<n-1;i++)
        { 
            c|=(buff[i+1]<<(8*(i+1)));
        }

    return c;
}

FatType TypeofFAT(FILE *fp){   
    FatType flag=FAT12;
    fseek(fp,0x36,SEEK_SET);
    // uint16_t *buffer=(uint16_t*)malloc(5*sizeof(uint16_t));
    char buffer[5];
    for(int i=0;i<5;i++)
    {
        buffer[i]=fgetc(fp);
    }

    if(buffer[3]=='1'&&buffer[4]=='2')
    {
        flag=FAT12;
    }
    else if(buffer[3]=='1'&&buffer[4]=='6')
    {
        flag=FAT16;
    }
    //offset 0x52
    else
        flag=FAT32;
   
   return flag;

}
char *Hex2Char(int n,int offset,FILE *fp)
{
    char *s=malloc(n*sizeof(char));
    fseek(fp,offset,SEEK_SET);
    for(int i=0;i<8;i++)
    {
        s[i]=fgetc(fp);
    }
    return s;
}
Boot Read_Boot(FILE *fp)
{
    Boot bootsector;
    bootsector.Fat_Type=TypeofFAT(fp);
    switch (bootsector.Fat_Type)
    {
    case 0:
    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
    bootsector.numfat=ReadnByte(1,NumFAT,fp);
    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
    bootsector.numentry=ReadnByte(2,NumEntry,fp);
    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
        break;
    case 1:
    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
    bootsector.numfat=ReadnByte(1,NumFAT,fp);
    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
    bootsector.numentry=ReadnByte(2,NumEntry,fp);
    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
    case 2: 
    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
    bootsector.numfat=ReadnByte(1,NumFAT,fp);
    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
    bootsector.numentry=ReadnByte(2,NumEntry,fp);
    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
    default:
        break;
    }

    return bootsector;
}
void Displayboot(Boot boot)
{
    printf("Kieu FAt:%d\n",boot.Fat_Type);
    printf("So Sector truoc Fat:%d\n",boot.secbeforefat);
    printf("So Sector moi Cluster:%d\n",boot.secperclus);
    printf("So Entry trong Root:%d\n",boot.numentry);
    printf("So luong Fat:%d\n",boot.numfat);
    printf("So luong Sec moi FAT:%d\n",boot.secperfat);
}
File* Read_File(FILE *fp)
{
    Boot boot =Read_Boot(fp);
    
    //find the Root Directory
    // Sec cua Root= So luong Fat * Kich thuoc Fat+ BootSector
    int RootSector=boot.numfat*boot.secperfat+boot.secbeforefat;
    //find subfolder and file
    // file
    int offset =0x20;
    int pointer=RootSector*0x200;
    for(int i=0;i<224;i++,pointer+=offset)
        {  
            while(ReadnByte(1,pointer+0x0b,fp)==0x0f)
                {
                    pointer+=offset;
                }
                file[i].name=Hex2Char(8,pointer+Filename,fp);
                file[i].extension=Hex2Char(3,pointer+NameExten,fp);
        }
    return file;
}
void main()
{   
    FILE *fp=fopen("floppy.img","r");
    File *file=Read_File(fp);
    printf("%s",file[223].name);

}
