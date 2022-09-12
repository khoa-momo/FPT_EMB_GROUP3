
#ifndef __FAT_H_
#define __FAT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "FAT.h"




////////////////////////BOOT OffSet////////////////////////////////////////
//////////////////////////////////////////////Byte//////////////
#define        SecPerClus     0x0du    // SC    1
#define        BytePerSec  0x0bu    //          2
#define        SecBeForeFAT  0x0eu      //SB    2
#define        NumFAT   0x10u     //Nf          1
#define        NumEntry  0x11u  //SR            2
#define        SecPerFat  0x16u //SF            2
#define        TypeofFat_12_16 0x36u      //    8  
#define        TypeofFat32   0x58u  //         8
//////////////////////FAT///////////////////////////////////////////

///////////////////////ROOTDIRECTORY////////////////////////////////
#define Filename  0x00u    //8
#define NameExten 0x08u    //3
#define FileAtrr  0x0bu    //1
#define TimeCreat 0x16u     //2
#define DateCreat 0x18u    //2
#define FisrtClus  0x1au     //2
#define FileSz     0x1cu    //4
/// FATTYPE////////////////////////////////////////////////////
typedef enum {
    FAT12=0,
    FAT16=1,
    FAT32=2
}FatType;

typedef struct File 
    {   
        // num of subfolder
        int numSub;
        //timecreated or last update 
        int daycreat;  //yyyyyyymmmmddddd
        //data size
        int datasize;
        char *name;
        int timecreat; //hhhhhmmmmmmxxxxx
        //Filename
        //extension
        char *extension;  
        //Cluster 
        int FirstClus;
    } File;  

typedef struct BootSector 
    {   //type of FAT
        FatType Fat_Type;
        //Number Sector per CLuster
        int secperclus;
        // Number FAT
        int numfat;
        //Size Sector
        int bytepersec;
        // Number of Entry per root directory
        int numentry;
        //number Fat before FAT
        int secbeforefat;
        //Sec per FAT
        int secperfat;
    } Boot;
typedef struct Node{
    File *file;
    struct Node *next;
}Node;
typedef struct LinkedList
{
	Node  *RootDirectory;
	Node  *tail;
}List;

Node *CreatNode(File *f);
void CreatList(List *l);
void DeleteFinalNode(List *l);
void AddNode(List *l,Node *p);
void DisplayList(List *l);

void Displayboot(Boot boot);
Boot Read_Boot(FILE *fp); // read boot
FatType Fat_Type(FILE *fp );// Check type of Fat



///tool
int ReadnByte(int,int,FILE *fp);
char *Hex2Char(int n,int, FILE *fp);
///

void Go2Folder_File(FILE *fp,List *l,File *f);
void Scan_Folder(File *f,FILE *fp,List *l);
File *ScanRoot(FILE *fp);
File *ScanFolder(File *f,FILE *fp);

#endif