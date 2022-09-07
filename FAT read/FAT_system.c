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

void getBootSector(FILE *fptr, Boot_Sector *Boot){
	int i;
	//1.FAT: 
	//0x36 -char
	Shift_Offset(BOOT_FAT_TYPE,0);
	for(i=0;i<BOOT_FAT_TYPE_BYTE;i++){
		Boot->FAT_type[i]=fgetc(fptr);
	}
	//0x10 -char
	Shift_Offset(BOOT_NUM_FAT,0);
	for(i=0;i<BOOT_NUM_FAT_BYTE;i++){
		Boot->num_FAT=fgetc(fptr);
	}
	//0x16 -int
	Shift_Offset(BOOT_SEC_PER_FAT,0);
	for(i=0;i<BOOT_SEC_PER_FAT_BYTE;i++){
		Boot->sec_per_FAT+=(fgetc(fptr)<<(8*i));
	}
	//0x10 -int
	Shift_Offset(BOOT_RSV_SEC_CNT,0);
	for(i=0;i<BOOT_RSV_SEC_CNT_BYTE;i++){
		Boot->resv_sec_cnt+=(fgetc(fptr)<<(8*i));
	}
	
	//2.Root
	//0x11 -int
	Shift_Offset(BOOT_ROOT_ENT_CNT,0); 
	for(i=0;i<BOOT_ROOT_ENT_CNT_BYTE;i++){
		Boot->root_entr_cnt+=(fgetc(fptr)<<(8*i));
	}
	
	//3.Data
	//0x0b - BLock/Sec Size - 512B -int
	Shift_Offset(BOOT_BYTE_PER_SEC,0);
	for(i=0;i<BOOT_BYTE_PER_SEC_BYTE;i++){
		Boot->byte_per_sec+=(fgetc(fptr)<<(8*i));
	}
	//0x0d - block/sector per cluster -char
	Shift_Offset(BOOT_SEC_PER_CLUS,0);
	for(i=0;i<BOOT_SEC_PER_CLUS_BYTE;i++){
		Boot->sec_per_clus=fgetc(fptr);
	} 
}

void getRootSector(FILE *fptr, Entr_Main_Root *Root, int rootDirect){
	int i;
	//Main entry
	//0x00 -char
	Shift_Offset(ENTRY_FILE_NAME+rootDirect,0);
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++){
		Root->name[i]=fgetc(fptr);
	} 
	//0x08 -char
	Shift_Offset(ENTRY_FILE_NAME_EXT+rootDirect,0);
	for(i=0;i<ENTRY_FILE_NAME_EXT_BYTE;i++){
		Root->ext[i]=fgetc(fptr);
	} 
	//0x0b -char
	Shift_Offset(ENTRY_FILE_ATTR+rootDirect,0);
	for(i=0;i<ENTRY_FILE_ATTR_BYTE;i++){
		Root->ext[i]=fgetc(fptr);
	}  
	//0x1a -int
	Shift_Offset(ENTRY_START_CLU_NUM+rootDirect,0);
	for(i=0;i<ENTRY_START_CLU_NUM_BYTE;i++){
		Root->first_clus+=(fgetc(fptr)<<(8*i));
	}
	//0x1c -long
	Shift_Offset(ENTRY_SIZE_FILE+rootDirect,0);
	for(i=0;i<ENTRY_SIZE_FILE_BYTE;i++){
		Root->size+=(fgetc(fptr)<<(8*i));
	} 
}

void displayBoot(Boot_Sector Boot){ 
	//1.FAT
	printf("FAT_type:%s\n",Boot.FAT_type);
	printf("Num_FAT:%d\n",Boot.num_FAT);
	printf("Resv_sec_cnt:%d\n",Boot.resv_sec_cnt);
	printf("Sec_per_FAT:%d\n",Boot.sec_per_FAT);
	
	//2.Root
	printf("Root_entr_cnt:%d\n",Boot.root_entr_cnt);
	
	//3.Data
	printf("Byte_per_sec:%d\n",Boot.byte_per_sec);
	printf("Sec_per_clus:%d\n",Boot.sec_per_clus);
	
	//printf("%mark:s\n",Boot.mark);
}
 
void displayRoot(Entr_Main_Root Root){  
	//3.Data
	printf("first_clus:%d\n",Root.first_clus);
	printf("size:%d\n",Root.size);
	
	//printf("%mark:s\n",Boot.mark);
}
  
int findRootDirectory(Boot_Sector Boot){
	int RootDirectory = (Boot.num_FAT * Boot.sec_per_FAT) + Boot.resv_sec_cnt;
	int RootDirectoryStart = RootDirectory * 512;
	printf("\nRootDirectory: %x\n",RootDirectory);
	printf("RootDirectoryStart: %x\n",RootDirectoryStart);
	
//	int RootBlock = (Boot.root_entr_cnt*32)/512; 
//	printf("\RootBlock: %d\n",RootBlock);
//	int ClusStart = RootDirectory+RootBlock-2;
	
	return (RootDirectoryStart+32);
} 

void findClusStart(Boot_Sector Boot, Entr_Main_Root Root){
	int RootDirectory = (Boot.num_FAT * Boot.sec_per_FAT) + Boot.resv_sec_cnt; 
	int RootBlock = (Boot.root_entr_cnt*32)/512; 
	printf("\RootBlock: %d\n",RootBlock);
	int ClusStart = RootDirectory+RootBlock-2;
	
	printf("ClusStart: %x\n",ClusStart);
	int data = Root.first_clus + ClusStart;
	printf("data: %x\n",data*0x200);
//	printf("return FAT: %x\n",(Root.first_clus*2)/0x200);
//	return data;
} 


#endif
