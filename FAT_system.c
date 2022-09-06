#ifndef	__FAT_SYSTEM_C__
#define	__FAT_SYSTEM_C__
#include"FAT_system.h"

void getByte(FILE *fptr,int byte){
	int i;
	int *arr;
	for(i=0;i<8;i++){
		arr[i]=(byte&(1<<i)?1:0);
	}
}

void getBootSector(FILE *fptr, Boot_Sector *Boot)
{
	int i;
	Shift_Offset(BOOT_BYTE_PER_SEC,0);
	for(i=0;i<BOOT_BYTE_PER_SEC_BYTE;i++){
		Boot->byte_per_sec+=(fgetc(fptr)<<(8*i));
	}
	Shift_Offset(BOOT_SEC_PER_CLUS,0);
	for(i=0;i<BOOT_SEC_PER_CLUS_BYTE;i++){
		Boot->sec_per_clus=fgetc(fptr);
	}
	Shift_Offset(BOOT_RSV_SEC_CNT,0);
	for(i=0;i<BOOT_RSV_SEC_CNT_BYTE;i++){
		Boot->resv_sec_cnt+=(fgetc(fptr)<<(8*i));
	}
	Shift_Offset(BOOT_NUM_FAT,0);
	for(i=0;i<BOOT_NUM_FAT_BYTE;i++){
		Boot->num_FAT=fgetc(fptr);
	}
	Shift_Offset(BOOT_ROOT_ENT_CNT,0);
	for(i=0;i<BOOT_ROOT_ENT_CNT_BYTE;i++){
		Boot->root_entr_cnt+=(fgetc(fptr)<<(8*i));
	}
	Shift_Offset(BOOT_SEC_PER_FAT,0);
	for(i=0;i<BOOT_SEC_PER_FAT_BYTE;i++){
		Boot->sec_per_FAT+=(fgetc(fptr)<<(8*i));
	}
	Shift_Offset(BOOT_FAT_TYPE,0);
	for(i=0;i<BOOT_FAT_TYPE_BYTE;i++){
		Boot->FAT_type[i]=fgetc(fptr);
	}

}
void displayBoot(Boot_Sector Boot){
	printf("Byte_per_sec:%d\n",Boot.byte_per_sec);
	printf("Sec_per_clus:%d\n",Boot.sec_per_clus);
	printf("Resv_sec_cnt:%d\n",Boot.resv_sec_cnt);
	printf("Num_FAT:%d\n",Boot.num_FAT);
	printf("Root_entr_cnt:%d\n",Boot.root_entr_cnt);
	printf("Sec_per_FAT:%d\n",Boot.sec_per_FAT);
	printf("FAT_type:%s\n",Boot.FAT_type);
	//printf("%mark:s\n",Boot.mark);
}


#endif
