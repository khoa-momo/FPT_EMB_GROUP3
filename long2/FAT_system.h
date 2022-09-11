#ifndef __FAT_SYSTEM_H__
#define __FAT_SYSTEM_H__

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

/////////////////////////////////////////////////////////////	
/*offset area boot sector*/
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

//The number of sector of FAT	- 2 BYTE 
#define BOOT_SEC_PER_FAT		(0x16U)
#define BOOT_SEC_PER_FAT_BYTE	(0x2U)

//file systeam identifier		- 8 BYTE
#define BOOT_FAT_TYPE			(0x36U)
#define BOOT_FAT_TYPE_BYTE		(0x8U)

//boot sector mark end boot		- 2 BYTE
#define BOOT_END_BOOT_SEC		(0x1FEU)
#define BOOT_END_BOOT_SEC_BYTE	(0x2U)
//end
/////////////////////////////////////////////////////////////
/*offset short name entry in root directory*/
#define	ENTRY_FILE_NAME				(0X00U)
#define	ENTRY_FILE_NAME_BYTE 		(0x08U)

#define	ENTRY_FILE_NAME_EXT			(0X08U)
#define	ENTRY_FILE_NAME_EXT_BYTE 	(0x03U)

#define	ENTRY_FILE_ATTR				(0X0bU)
#define	ENTRY_FILE_ATTR_BYTE		(0x01U)

#define	ENTRY_RESERVED				(0x0cU)
#define	ENTRY_RESERVED_BYTE			(0x0aU)

#define	ENTRY_START_CLU_HIG			(0x14U)
#define	ENTRY_START_CLU_HIG_BYTE	(0x02U)

#define	ENTRY_TIME_CRE_UPD			(0x16U)
#define	ENTRY_TIME_CRE_UPD_BYTE		(0x02U)

#define	ENTRY_DATE_CRE_UPD			(0x18U)
#define	ENTRY_DATE_CRE_UPD_BYTE		(0x02U)

#define	ENTRY_START_CLU_LOW			(0x1aU)
#define	ENTRY_START_CLU_LOW_BYTE	(0x02U)

#define	ENTRY_SIZE_FILE				(0x1cU)
#define	ENTRY_SIZE_FILE_BYTE		(0x04U)

/*offset long name entry in root directory*/
#define ENTRY_SEQUE_NUM				(0x00U)
#define ENTRY_SEQUE_NUM_BYTE		(0x01U)

#define ENTRY_FIR_FIV_CHAR			(0x01U)
#define ENTRY_FIR_FIV_CHAR_BYTE		(0x0aU)

#define ENTRY_FLAG					(0x0bU)
#define ENTRY_FLAG_BYTE				(0x01U)

#define ENTRY_RESEVSER				(0x0cU)
#define ENTRY_RESEVSER_BYTE			(0x01U)

#define ENTRY_CHECKSUM				(0x0dU)
#define ENTRY_CHECKSUM_BYTE			(0x01U)

#define ENTRY_NEX_SIX_CHAR			(0x0eU)
#define ENTRY_NEX_SIX_CHAR_BYTE		(0x0cU)

#define ENTRY_RESEVER_2				(0x1aU)
#define ENTRY_RESEVER_2_BYTE		(0x02U)

#define ENTRY_LAS_TWO_CHAR			(0x1cU)
#define ENTRY_LAS_TWO_CHAR_BYTE		(0x04U)

/////////////////////////////////////////////////////////////////

/*file attribute*/

/**entry in FAT**/
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

///////////////////////////////////////////////////////////////
//shift to offset
#define Shift_Offset(x)		fseek(fp,x,0)	


typedef struct Boot_Sector{
	int byte_per_sec;
	char sec_per_clus;
	int resv_sec_cnt;
	char num_FAT;
	int root_entr_cnt;
	int sec_per_FAT;
	char FAT_type[8];
	//int unkown;
	//char mark[2];
}Boot_Sector;

//entry main in root directory
typedef struct Entr_Main_Root{
	char name[8];
	int unkown; 
	char ext[3];
	char attr;
	int first_clus_hig;
	//char reserver[10];
	//char time[2];
	//char date[2];
	int first_clus_low;
	//long size;
}Entry_Short;

typedef struct{
	char seque_num;
	char fir_fiv_char[10];
	char flag;
	char checksum;
	char nex_six_char[12];
	char las_two_char[4];
}Entr_Long;

FILE*fp;

//declared

void getBootSector(Boot_Sector *Boot);
Boot_Sector *readBootSector();
void displayBoot(Boot_Sector Boot);
void getShortEntry(Entry_Short *entr_sh,int offset);
Entry_Short *readEntryShort(int offset,int size_root,int *cnt_entr_sh);
void displayEntryShort(Entry_Short *entr_sh,int cnt_entr_sh,int);
void print_Str(char str[],int size );
void readEntrInClus(Entry_Short *entr_sh,int offset, int *cnt_entr_sh);
void readData();
void checkFile(Entry_Short **entr_sh,int *cnt_entr_sh,int i);


#endif
