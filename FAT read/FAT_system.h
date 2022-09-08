#ifndef __FAT_SYSTEM_H__
#define __FAT_SYSTEM_H__

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
 
//------------------------Important Boot Feature------------------------
//File System Identifier FAT_TYPE - 8 BYTE
#define BOOT_FAT_TYPE			(0x36U)
#define BOOT_FAT_TYPE_BYTE		(0x8U)

//Number of FAT 				- 1 BYTE
#define BOOT_NUM_FAT			(0x10U)
#define BOOT_NUM_FAT_BYTE		(0x1U)

//Number of sector of FAT	- 2 BYTE 
#define BOOT_SEC_PER_FAT		(0x16U) //*
#define BOOT_SEC_PER_FAT_BYTE	(0x2U)

//Number of reserved sector (Bef FAT) - 2 BYTE
#define BOOT_RSV_SEC_CNT		(0x0eU)
#define BOOT_RSV_SEC_CNT_BYTE	(0x2U)

 
//Number of RDRET. 				- 2 BYTE
#define BOOT_ROOT_ENT_CNT		(0x11U)
#define BOOT_ROOT_ENT_CNT_BYTE	(0x2U)
  
 
//Number of bytes per block/sector. - 2 BYTE  //usually 512B
#define BOOT_BYTE_PER_SEC		(0x0bU)
#define BOOT_BYTE_PER_SEC_BYTE	(0x2U) 

//Number of block/sector per cluster. - 1 BYTE
#define BOOT_SEC_PER_CLUS		(0x0dU)
#define BOOT_SEC_PER_CLUS_BYTE	(0x1U) 


//----------------------------------------------------------------------
//Offset area boot sector
//BOOTSTRAP - 3 BYTE
#define BOOT_BOOTSP				(0x00U)
#define BOOT_BOOTSP_BYTE		(0x3U) 
//Optional manufacturer description - 8 BYTE
#define BOOT_OEM_NAME			(0x03U)
#define BOOT_OEM_NAME_BYTE		(0x8U)

//Total number of sector		- 2 BYTE
#define BOOT_SEC_VOL			(0x13U)
#define BOOT_SEC_VOL_BYTE		(0x2U)
 
//Media desciptor				- 1 BYTE
#define BOOT_MEDIA				(0x15U)
#define BOOT_MEDIA_BYTE			(0x1U)

//The number of sector per track- 2 BYTE
#define BOOT_SEC_PER_TRK		(0x18U)
#define BOOT_SEC_PER_TRK_BYTE	(0x2U)

//The number of heads			- 2 BYTE
#define BOOT_NUM_HEAD			(0x1aU)
#define BOOT_NUM_HEAD_BYTE		(0x2U)

//The number of hidden sector	- 4 BYTE
#define BOOT_HIDD_SEC			(0x1cU)
#define BOOT_HIDD_SEC_BYTE		(0x4U)

//Total sector in entire disk	- 4 BYTE
#define BOOT_SZ_VOL				(0x20U)
#define BOOT_SZ_VOL_BYTE		(0x4U)

//Physical diver number			- 2 BYTE
#define BOOT_NUM_DRV			(0x24U)
#define BOOT_NUM_DRV_BYTE		(0x2U)

//Extended Boot Record Signature- 1 BYTE
#define BOOT_RECD_SIGNE			(0x26U)
#define BOOT_RECD_SIGNE_BYTE	(0x1U)

//Volume serial number			- 4 BYTE
#define BOOT_VOL_ID				(0x27U)
#define BOOT_VOL_ID_BYTE		(0x4U)

//Volume label					- 11 BYTE
#define BOOT_VOL_LABEL			(0x2bU)
#define BOOT_VOL_LABEL_BYTE		(0xbU)
 
//Remainder of bootstrap.		- 448 BYTE
#define BOOT_REMD_BOOTSP		(0x3eU)
#define BOOT_REMD_BOOTSP_BYTE	(0x1c0U)

//boot sector mark end boot		- 2 BYTE
#define BOOT_END_BOOT_SEC		(0x1FEU)
#define BOOT_END_BOOT_SEC_BYTE	(0x2U)
//----------------------------end----------------------------

//-------------------Important Root Feature-------------------
//Offset Main Entry In Root Directory
#define	ENTRY_FILE_NAME				(0X00U)
#define	ENTRY_FILE_NAME_BYTE 		(0x8U)

#define	ENTRY_FILE_NAME_EXT			(0X08U)
#define	ENTRY_FILE_NAME_EXT_BYTE 	(0x3U)

#define	ENTRY_FILE_ATTR				(0X0bU)
#define	ENTRY_FILE_ATTR_BYTE		(0x1U)

#define	ENTRY_RESERVED				(0x0cU)
#define	ENTRY_RESERVED_BYTE			(0xaU)

#define	ENTRY_TIME_CRE_UPD			(0x16U)
#define	ENTRY_TIME_CRE_UPD_BYTE		(0x2U)

#define	ENTRY_DATE_CRE_UPD			(0x18U)
#define	ENTRY_DATE_CRE_UPD_BYTE		(0x2U)

#define	ENTRY_START_CLU_NUM			(0x1aU)
#define	ENTRY_START_CLU_NUM_BYTE	(0x2U)

#define	ENTRY_SIZE_FILE				(0x1cU)
#define	ENTRY_SIZE_FILE_BYTE		(0x4U)

//shift to offset
#define Shift_Offset(x,y)		fseek(fptr,x,y)	

typedef struct Boot_Sector{
	//1.Fat: Fat_Type; no_of_FAT
	char FAT_type[8];		//0x36 (5B)
	char num_FAT; 			//0x10 (1B)
	int resv_sec_cnt; 		//0x0e (2B)
	int sec_per_FAT; 		//0x16 (2B)
	
	//2.Root: no_of_Root_Entry (Root entry count)
	int root_entr_cnt;	 	//0x11 (2B)
	
	//3.Data: Byte_per_sector; sector_per_cluster, no_of_resv_cluster
	int byte_per_sec; 		//0x0b (2B) |Sector/Block Size
	char sec_per_clus; 		//0x0d (2B)
	  
	//char mark[2];
}Boot_Sector;

//entry main in root directory
typedef struct Entr_Main_Root{//32 byte
	char name[8];			//0x00	//File name (8B)
	char ext[3];			//0x08	//File extension (3B)
	char attr;				//0x0b	//File attribute (1B)
	char reserver[10];		//0x0c	//Reserved (10B)
	char time[2];			//0x16	//Time created or last updated (2B)
	char date[2];			//0x18	//Date created or last updated (2B)
	int first_clus;			//0x1a	//*Start cluster number for file (2B) 
	long size;				//0x1c	//*File size in Byte	(4B)		
}Entr_Main_Root;


#endif
