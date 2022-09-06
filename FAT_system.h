#ifndef __FAT_SYSTEM_H__
#define __FAT_SYSTEM_H__

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

	

//offset area boot sector
//BOOTSTRAP - 3 BYTE
#define BOOT_BOOTSP				(0x00U)
#define BOOT_BOOTSP_BYTE		(0x3U)

//Optional manufacturer description - 8 BYTE
#define BOOT_OEM_NAME			(0x03U)
#define BOOT_OEM_NAME_BYTE		(0x8U)

//Number of bytes per sector. 	- 2 BYTE
#define BOOT_BYTE_PER_SEC		(0x0bU)
#define BOOT_BYTE_PER_SEC_BYTE	(0x2U)

//Number of blocks per cluster. - 1 BYTE
#define BOOT_SEC_PER_CLUS		(0x0dU)
#define BOOT_SEC_PER_CLUS_BYTE	(0x1U)

//Number of reserved sector. 	- 2 BYTE
#define BOOT_RSV_SEC_CNT		(0x0eU)
#define BOOT_RSV_SEC_CNT_BYTE	(0x2U)

//Number of FAT 				- 1 BYTE
#define BOOT_NUM_FAT			(0x10U)
#define BOOT_NUM_FAT_BYTE		(0x1U)

//Number of RDRET. 				- 2 BYTE
#define BOOT_ROOT_ENT_CNT		(0x11U)
#define BOOT_ROOT_ENT_CNT_BYTE	(0x2U)

//Total number of sector		- 2 BYTE
#define BOOT_SEC_VOL			(0x13U)
#define BOOT_SEC_VOL_BYTE		(0x2U)

//Media desciptor				- 1 BYTE
#define BOOT_MEDIA				(0x15U)
#define BOOT_MEDIA_BYTE			(0x1U)

//The number of sector of FAT	- 2 BYTE 
#define BOOT_SEC_PER_FAT		(0x16U)
#define BOOT_SEC_PER_FAT_BYTE	(0x2U)

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

//file systeam identifier		- 8 BYTE
#define BOOT_FAT_TYPE			(0x36U)
#define BOOT_FAT_TYPE_BYTE		(0x8U)

//Remainder of bootstrap.		- 448 BYTE
#define BOOT_REMD_BOOTSP		(0x3eU)
#define BOOT_REMD_BOOTSP_BYTE	(0x1c0U)

//boot sector mark end boot		- 2 BYTE
#define BOOT_END_BOOT_SEC		(0x1FEU)
#define BOOT_END_BOOT_SEC_BYTE	(0x2U)
//end


//offset main entry in root directory
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
	int byte_per_sec;//
	char sec_per_clus;//
	int resv_sec_cnt;//
	char num_FAT;//
	int root_entr_cnt;//
	int sec_per_FAT;//
	char FAT_type[8];//
	//char mark[2];
}Boot_Sector;

//entry main in root directory
typedef struct Entr_Main_Root{
	char name[8];
	char ext[3];
	char attr;
	char reserver[10];
	char time[2];
	char date[2];
	int first_clus;
	long size;
}Entry_Main;
	

#endif
