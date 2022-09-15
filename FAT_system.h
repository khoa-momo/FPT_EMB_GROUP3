#ifndef __FAT_SYSTEM_H__
#define __FAT_SYSTEM_H__

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

/******************************************************************/	
/******************** Offset Boot Sector *************************/
//File Systeam Identifier		- 8 BYTE
#define BOOT_FAT_TYPE			(0x36U)
#define BOOT_FAT_TYPE_BYTE		(0x8U)

//Number of FAT. 				- 1 BYTE
#define BOOT_NUM_FAT			(0x10U)
#define BOOT_NUM_FAT_BYTE		(0x1U)

//Number of Sector of FAT.		- 2 BYTE 
#define BOOT_SEC_PER_FAT		(0x16U)
#define BOOT_SEC_PER_FAT_BYTE	(0x2U)

//Number of reserved sector (Bef FAT). - 2 BYTE
#define BOOT_RSV_SEC_CNT		(0x0eU)
#define BOOT_RSV_SEC_CNT_BYTE	(0x2U)


//Number of RDRET. 				- 2 BYTE
#define BOOT_ROOT_ENT_CNT		(0x11U)
#define BOOT_ROOT_ENT_CNT_BYTE	(0x2U)

//Number of Bytes per Sector. 	- 2 BYTE //usually 512B
#define BOOT_BYTE_PER_SEC		(0x0bU)
#define BOOT_BYTE_PER_SEC_BYTE	(0x2U)

//Number of blocks per Cluster. - 1 BYTE
#define BOOT_SEC_PER_CLUS		(0x0dU)
#define BOOT_SEC_PER_CLUS_BYTE	(0x1U)

//Boot Sector Mark End Boot.		- 2 BYTE
#define BOOT_END_BOOT_SEC		(0x1FEU)
#define BOOT_END_BOOT_SEC_BYTE	(0x2U)
/****************************** Boot Sector *******************************/

/************************ Offset Short Name Entry ***********************/
//File Name
#define	ENTRY_FILE_NAME				(0X00U)
#define	ENTRY_FILE_NAME_BYTE 		(0x08U)

//File Extension
#define	ENTRY_FILE_NAME_EXT			(0X08U)
#define	ENTRY_FILE_NAME_EXT_BYTE 	(0x03U)

//File Attribute
#define	ENTRY_FILE_ATTR				(0X0bU)
#define	ENTRY_FILE_ATTR_BYTE		(0x01U)

//First Cluster of A File (High)
#define	ENTRY_START_CLU_HIG			(0x14U)
#define	ENTRY_START_CLU_HIG_BYTE	(0x02U)

//File Lastest Update Time
#define	ENTRY_TIME_CRE_UPD			(0x16U)
#define	ENTRY_TIME_CRE_UPD_BYTE		(0x02U)

//File Lastest Update Date
#define	ENTRY_DATE_CRE_UPD			(0x18U)
#define	ENTRY_DATE_CRE_UPD_BYTE		(0x02U)

//First Cluster of A File (Low)
#define	ENTRY_START_CLU_LOW			(0x1aU)
#define	ENTRY_START_CLU_LOW_BYTE	(0x02U)

//File Size
#define	ENTRY_SIZE_FILE				(0x1cU)
#define	ENTRY_SIZE_FILE_BYTE		(0x04U)

#define ENTRY_FLAG					(0x0bU)
#define ENTRY_FLAG_BYTE				(0x01U)
/*********************** Offset Short Name Entry ***********************/

/*************************** Entry In FAT ****************************/
/*FAT 12*/
#define FAT12_FREE				(0x000U)
#define FAT12_ERROR				(0xff7U)
#define FAT12_EOF				(0xfffU)
/*FAT 16*/
#define FAT16_FREE				(0x0000U)
#define FAT16_ERROR				(0xfff7U)
#define FAT16_EOF				(0xffffU)
/*FAT 32*/
#define FAT32_FREE				(0x00000000U)
#define FAT32_ERROR				(0x0ffffff7U)
#define FAT32_EOF				(0x0fffffffU)					
/*************************** Entry In FAT **************************/
/********************************************************************/

/************* Shift To Offset **************/
#define Shift_Offset(x)				fseek(fp,x,0)	
#define offset_Root(SF,NF,BF)       (((SF*NF)+BF)*0x200)
/***************** Struct ****************/
typedef enum bool{
	true=1, false=0
}bool;

typedef struct Boot_Sector{
	//Fat attr
	char FAT_type[8];		//0x36 (5B)
	char num_FAT;			//0x10 (1B)
	int sec_per_FAT;		//0x16 (2B)
	int resv_sec_cnt;		//0x0e (2B)
	//Root attr
	int root_entr_cnt;	 	//0x11 (2B) 
	//Data attr
	int byte_per_sec; 		//0x0b (2B) |Sector/Block Size -512B
	char sec_per_clus; 		//0x0d (2B)   
}Boot_Sector;

typedef enum FAT{
	FAT12=1,
	FAT16=2,
	FAT32=3
}FAT;

//entry main in root directory
typedef struct Entr_Main_Root{
	//data
	char name[8];			//0x00 - File name (8B)
	char ext[3];			//0x08 - File extension (3B)
	char attr;				//0x0b - File attribute (1B)
	int first_clus_hig;		//0x14 - Start cluster number of file - HIGH (2B)
	//char reserver[10];
//	char time[2];			//0x16 - Time last updated (2B)
//	char date[2];			//0x18 - Date last updated (2B)
	
	//timecreated or last update 
	//hhhhhmmmmmmxxxxx
	//yyyyyyymmmmddddd
	//int timeCreate; 
	//int dateCreate;  
    int timeUpdate; 		//0x16 - Time last updated (2B)
    int dateUpdate;  		//0x18 - Date last updated (2B)
    
	int first_clus_low;		//0x1a - Start cluster number of file - LOW (2B) 
	//long size;

}Entry_Short;

/******************************* Declared Functions *******************************/

uint16_t GetFatValue12( uint16_t PrsClus);
uint16_t GetFatValue16( uint16_t PrsClus);


void getBootSector(Boot_Sector *Boot);
Boot_Sector *readBootSector();
void displayBoot(Boot_Sector Boot);

void getShortEntry(Entry_Short *entr_sh,int offset);
void countEntryShort(int offset,int *cnt_entr_sh);
Entry_Short *readEntryShort(int offset,int *cnt_entr_sh);
void countEntryShortClus(int offset, int *cnt_entr_sh, int locat_clus);
Entry_Short *readEntrInClus(int offset, int *cnt_entr_sh,int locat_clus);
void displayEntryShort(Entry_Short *entr_sh,int cnt_entr_sh,int);

void print_Str(char str[],int size );
void prtByteFile(int n);
void readDate(int raw_date);
void readTime(int raw_time);
int offset_clus(int first_clus_low);
void main_offset();

/******************************** Declared ********************************/

/********************************* LINK LIST **********************************/
typedef struct Node{
	Entry_Short entr_sh;
	struct Node *next;
} Node;

/************** Declared Node **************/
void initList();
bool isEmpty();
void addNodeToList(Node*node);
void removeNode();
void createNode(Entry_Short *entr_sh);
void printLinkedList(Node *head);
/********************************* LINK LIST **********************************/

/***************global_variable***************/ 
FILE *fp; //FILE pointer
Boot_Sector *Boot;
Node *head; 

int Root_offset;
int bef_data_sector;
/***************global_variable***************/ 
#endif
