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

void getRootSector(FILE *fptr, Entr_Main_Root *Root, int startIndex){
	int i;
	//Main entry
	//0x00 -char
	printf("|||||||%x|||||||\n",ENTRY_START_CLU_NUM +startIndex);
	Shift_Offset(ENTRY_FILE_NAME+startIndex,0);
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++){
		Root->name[i]=fgetc(fptr);
	} 
	//0x08 -char
	Shift_Offset(ENTRY_FILE_NAME_EXT+startIndex,0);
	for(i=0;i<ENTRY_FILE_NAME_EXT_BYTE;i++){
		Root->ext[i]=fgetc(fptr);
	} 
	//0x0b -char
	Shift_Offset(ENTRY_FILE_ATTR+startIndex,0);
	for(i=0;i<ENTRY_FILE_ATTR_BYTE;i++){
		Root->attr=fgetc(fptr);
	}  
	//0x1a -int
	Shift_Offset(ENTRY_START_CLU_NUM+startIndex,0);
	for(i=0;i<ENTRY_START_CLU_NUM_BYTE;i++){
		Root->first_clus+=(fgetc(fptr)<<(8*i));
	}
	//0x1c -long
	Shift_Offset(ENTRY_SIZE_FILE+startIndex,0);
	for(i=0;i<ENTRY_SIZE_FILE_BYTE;i++){
		Root->size+=(fgetc(fptr)<<(8*i));
	} 
}

void displayBoot(Boot_Sector Boot){ 
	//1.FAT
	printf("#1.BOOT ATTRIBUTE\n");
	printf("FAT_type:%s\n",Boot.FAT_type);
	printf("Num_FAT:%d\n",Boot.num_FAT);
	printf("Resv_sec_cnt:%d\n",Boot.resv_sec_cnt);
	printf("Sec_per_FAT:%d\n",Boot.sec_per_FAT); 
	//2.Root
	printf("Root_entr_cnt:%d\n",Boot.root_entr_cnt);
	//3.Data
	printf("Byte_per_sec:%d\n",Boot.byte_per_sec);
	printf("Sec_per_clus:%d\n\n",Boot.sec_per_clus);
	 
}
    
int findRootDirectory(Boot_Sector Boot){
	int RootDirectory = (Boot.num_FAT * Boot.sec_per_FAT) + Boot.resv_sec_cnt;
	int RootDirectoryStart = RootDirectory * 512;
	printf("RootDirectory: %x\n",RootDirectory);//31
	printf("RootDirectoryStart: %x\n",RootDirectoryStart);//2600
//	int RootBlock = (Boot.root_entr_cnt*32)/512; 
//	printf("\RootBlock: %d\n",RootBlock);
//	int ClusStart = RootDirectory+RootBlock-2;
	return (RootDirectoryStart+32);
}  

void displayRoot(Entr_Main_Root Root){  
	//3.Data
	printf("\n#2.ROOT ATTRIBUTE\n");
	printf("name:%s\n",Root.name);
	printf("First_cluster:%d\n",Root.first_clus);
	printf("Size:%d\n",Root.size);
	
	//printf("%mark:s\n",Boot.mark);
}

void findClusStart(Boot_Sector Boot, Entr_Main_Root Root){
	int RootDirectory = (Boot.num_FAT * Boot.sec_per_FAT) + Boot.resv_sec_cnt; 
	int RootBlock = (Boot.root_entr_cnt*32)/512; 
	printf("\nRoot_entr_cnt:%d; RootBlock: %d\n",Boot.root_entr_cnt, RootBlock);
	int ClusStart = RootDirectory+RootBlock-2;
	
	printf("Data_Cluster_Start: %d\n",ClusStart);//31
	int data = Root.first_clus + ClusStart;//3 + 31
	printf("data:%d; data:%d-%x\n",data,data*0x200,data*0x200);
	
	
//	printf("return FAT: %x\n",(Root.first_clus*2)/0x200);
//	return data;
} 

//void READROOT(FILE *fptr, Boot_Sector Boot, Entr_Main_Root Root){ 
//	char temp;
//	for(int i = 0; i < 32 * Boot.root_entr_cnt; i+=32){
//		fseek(fptr, 0x2600 + i, SEEK_SET);
////	   	SET_FILE_POITER(0x2600 + i);
//	    if((temp = fgetc(fptr) != NULL)){
//	    	
//	    	fseek(fptr, 0x2600 + 0x0B + i, SEEK_SET);
////	       	SET_FILE_POITER(0x2600 + 0x0B + i);
//	       	
//	       	if((temp = fgetc(fptr)) == 0x0F){
//	           /* Do Nothing */
//	       	}
//	       	else{
//	           	Root.first_clus = 0;
//	           	fseek(fptr, 0x2600 + 0x1A + i, SEEK_SET);
////	           	SET_FILE_POITER(0x2600 + 0x1A + i);
//	           	for(int i = 0; i < 2; i++){
//	               	Root.first_clus += (fgetc(fptr) << (8 * i));
//	           	}printf("%x\n", Root.first_clus);
//	       	}
//	   	}
//		else{
//	       	break;
//	   	}
//	}
//}



Entr_Main_Root *foo2(FILE *fptr, Boot_Sector *Boot, int n) { 
    // creating array of n numbers
    Entr_Main_Root *Root = (Entr_Main_Root*)malloc(n * sizeof(Entr_Main_Root));;
    // do something with array
//    int count = 0;
    
    for(int count = 0; count<n; count++){
//    	(array+i) -> num = i;
//		fseek(fptr, 0x2600+(count*32), SEEK_SET);
		int index = 0x2600+(count*32);
		printf("|||||||%x|||||||\n",ENTRY_START_CLU_NUM +index);
		
//		int whatever;
//		Shift_Offset(ENTRY_START_CLU_NUM +index,0);
//		for(int i=0; i<ENTRY_START_CLU_NUM_BYTE; i++){
//			whatever+=(fgetc(fptr)<<(8*i));
//		}
//		printf("||whatever: %x||\n",whatever);
		
		int whatever2;	
		Shift_Offset(ENTRY_START_CLU_NUM +index,0);
		for(int i=0; i<2; i++){
			whatever2=(fgetc(fptr)<<(8*i));
		}
		printf("||whatever2: %x||\n",whatever2);
		
		int i; 
		//Main entry
		//0x00 -char
		Shift_Offset(ENTRY_FILE_NAME +index,0);
		for(i=0; i<ENTRY_FILE_NAME_BYTE; i++){
			(Root+count) ->name[i]=fgetc(fptr);
		} 
		//0x08 -char
		Shift_Offset(ENTRY_FILE_NAME_EXT +index,0);
		for(i=0; i<ENTRY_FILE_NAME_EXT_BYTE; i++){
			(Root+count) ->ext[i]=fgetc(fptr);
		} 
		//0x0b -char
		Shift_Offset(ENTRY_FILE_ATTR +index,0);
		for(i=0; i<ENTRY_FILE_ATTR_BYTE; i++){
			(Root+count) ->attr=fgetc(fptr);
		}  
		//0x1a -int
		Shift_Offset(ENTRY_START_CLU_NUM +index,0);
		for(i=0; i<ENTRY_START_CLU_NUM_BYTE; i++){
			(Root+count) ->first_clus+=(fgetc(fptr)<<(8*i));
		}
		//0x1c -long
		Shift_Offset(ENTRY_SIZE_FILE +index,0);
		for(i=0; i<ENTRY_SIZE_FILE_BYTE; i++){
			(Root+count) ->size+=(fgetc(fptr)<<(8*i));
		} 
//		count++;
	}
	    
    
    
    
    return Root;
}


//void READROOT_All(FILE *fptr, Boot_Sector Boot, Entr_Main_Root Root){ 
//	char temp;
//	for(int i = 0; i < 32 * Boot.root_entr_cnt; i+=32){
//		//1.
//		fseek(fptr, 0x2600 + i, SEEK_SET); 
//	    if((temp = fgetc(fptr) != NULL)){
//	    	
//	    	fseek(fptr, 0x2600 + 0x0B + i, SEEK_SET); 
//	       	
//	       	if((temp = fgetc(fptr)) == 0x0F){
//	           /* Do Nothing */
//	       	}
//	       	else{
//	           	Root.first_clus = 0;
//	           	fseek(fptr, 0x2600 + 0x1A + i, SEEK_SET); 
//	           	for(int i = 0; i < 2; i++){
//	               	Root.first_clus += (fgetc(fptr) << (8 * i));
//	           	}printf("%x\n", Root.first_clus);
//	       	}
//	   	}
//		else{
//	       	break;
//	   	}
//	}
//}
  
#endif
