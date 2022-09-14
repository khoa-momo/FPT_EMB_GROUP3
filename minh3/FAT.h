#ifndef __FAT_H_
#define __FAT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*~~~~~~~~~~~~~~~~~~~~ BOOT ~~~~~~~~~~~~~~~~~~~~*/
#define        SecPerClus     	0x0du    	//SC   	1
#define        BytePerSec  		0x0bu    	//     	2
#define        SecBeForeFAT  	0x0eu      	//SB    2
#define        NumFAT   		0x10u     	//Nf    1
#define        NumEntry  		0x11u  		//SR  	2
#define        SecPerFat  		0x16u 		//SF  	2	
#define        TypeofFat_12_16 	0x36u    	//    	8  
#define        TypeofFat32   	0x58u  		//     	8

/*~~~~~~~~~~~~~~~~~ ROOT - MAIN ENTRY ~~~~~~~~~~~~~~~~~*/
#define Filename  (0x00U)    	//8
#define NameExten (0x08U)    	//3
#define FileAtrr  (0x0bU)    	//1

#define TimeCreate  (0x0dU)    	//3
#define DateCreate  (0x10U)    	//2

#define TimeUpdate (0x16U)     	//2
#define DateUpdate (0x18U)    	//2

#define FisrtClus  (0x1aU)     	//2
#define FileSz     (0x1cU)    	//4

/*~~~~~~~~~~~~~~~~~ FATTYPE ~~~~~~~~~~~~~~~~~*/
typedef enum {
    FAT12=0,
    FAT16=1,
    FAT32=2
} FatType;

/* I.BOOT */
typedef struct BootSector {   //type of FAT
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

/* II.ROOT - ENTRY */
typedef struct File {   
    // num of subfolder
    int numSub;
    
    //timecreated or last update 
    int timeCreate; //hhhhhmmmmmmxxxxx
	int dateCreate;  //yy yy yyy mm mm dd dd d
    int timeUpdate; //hhhhhmmmmmmxxxxx
    int dateUpdate;  //yy yy yyy mm mm dd dd d
    
	//data size
    int datasize;
    char *name;
    
    //extension
    char *extension; 
	 
    //Cluster 
    int FirstClus;
} File;  


/* Node */
typedef struct Node {
    File *file; //An struct array
    struct Node *next; //pointer
} Node;

typedef struct LinkedList {
	Node *RootDirectory;
	Node *tail;
} List;

//
Node *CreatNode(File *f);

void CreatList(List *l);
void DeleteFinalNode(List *l);
void AddNode(List *l,Node *p);
void DisplayList(List *l);

//
void Displayboot(Boot boot);
Boot Read_Boot(FILE *fp); // read boot
FatType Fat_Type(FILE *fp );// Check type of Fat

///tool
int ReadnByte(int,int,FILE *fp);
char *Hex2Char(int n,int, FILE *fp);

void Go2Folder_File(FILE *fp,List *l,File *f);
void Scan_Folder(File *f,FILE *fp,List *l);
File *ScanRoot(FILE *fp);
File *Scan_Sub_Folder(File *f,FILE *fp);

int count_Entry(int pointer,FILE *fp);

//time&date reading
void readDate(int raw_date);
void readTime(int raw_time);


#endif
