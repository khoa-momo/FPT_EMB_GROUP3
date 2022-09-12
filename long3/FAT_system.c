#ifndef	__FAT_SYSTEM_C__
#define	__FAT_SYSTEM_C__
#include"FAT_system.h"

void print_Str(char str[],int size ){
	int i;
	for(i=0;i<size;i++){
		printf("%c",str[i]);
	}
}

///////////////////////Boot_sector/////////////////////////
Boot_Sector *readBootSector(){
	Boot_Sector *Boot=(Boot_Sector*)calloc(1,sizeof(Boot_Sector));
	getBootSector(Boot);
	return Boot;
}

void getBootSector(Boot_Sector *Boot){
	int i;
	Shift_Offset(BOOT_BYTE_PER_SEC);
	for(i=0;i<BOOT_BYTE_PER_SEC_BYTE;i++){
		Boot->byte_per_sec+=(fgetc(fp)<<(8*i));
	}
	Shift_Offset(BOOT_SEC_PER_CLUS);
	for(i=0;i<BOOT_SEC_PER_CLUS_BYTE;i++){
		Boot->sec_per_clus=fgetc(fp);
	}
	Shift_Offset(BOOT_RSV_SEC_CNT);
	for(i=0;i<BOOT_RSV_SEC_CNT_BYTE;i++){
		Boot->resv_sec_cnt+=(fgetc(fp)<<(8*i));
	}
	Shift_Offset(BOOT_NUM_FAT);
	for(i=0;i<BOOT_NUM_FAT_BYTE;i++){
		Boot->num_FAT=fgetc(fp);
	}
	Shift_Offset(BOOT_ROOT_ENT_CNT);
	for(i=0;i<BOOT_ROOT_ENT_CNT_BYTE;i++){
		Boot->root_entr_cnt+=(fgetc(fp)<<(8*i));
	}
	Shift_Offset(BOOT_SEC_PER_FAT);
	for(i=0;i<BOOT_SEC_PER_FAT_BYTE;i++){
		Boot->sec_per_FAT+=(fgetc(fp)<<(8*i));
	}
	Shift_Offset(BOOT_FAT_TYPE);
	for(i=0;i<BOOT_FAT_TYPE_BYTE;i++){
		Boot->FAT_type[i]=fgetc(fp);	
	}
}

void displayBoot(Boot_Sector Boot){
	printf("#1.BOOT ATTRIBUTE\n");
	//1.FAT
	printf("FAT_type: ");print_Str(Boot.FAT_type,BOOT_FAT_TYPE_BYTE);
	printf("Num_FAT: %d\n",Boot.num_FAT);
	printf("Resv_sec_cnt: %d\n",Boot.resv_sec_cnt);
	printf("Sec_per_FAT: %d\n",Boot.sec_per_FAT);
	//2.Root
	printf("Root_entr_cnt: %d\n",Boot.root_entr_cnt);
	//3.Data
	printf("Byte_per_sec: %d\n",Boot.byte_per_sec);
	printf("Sec_per_clus: %d\n",Boot.sec_per_clus);
	printf("\n"); 
}////////////////////////////////////////////////////

//////////////////Root: readEntryShort////////////////////////
Entry_Short *readEntryShort(int offset,int size_root,int *cnt_entr_sh){
	printf("\n\nReadEntryShort\n");
	Entry_Short *entr_sh;
	int entr_locat = 0, flag;
	int temp = 1;
	
	printf("offset:%x |entr_locat:%x |size_root: %d\n\n",offset, entr_locat, size_root);
	while(entr_locat<size_root && temp!=0){
		
		//offset:0x2600
		/*//Find Main Entry - Since Sub Entry stand before Main entry
		//0x0f -> Sub Entry/Long Entry*/
		do{
			entr_locat++;
			printf("offset+0x20*(entr_locat-1): %x\n",offset+0x20*(entr_locat-1));
			//entr_locat-1 so the 1st index of entr_locat can start from 0
			Shift_Offset(offset+0x20*(entr_locat-1));
			temp = fgetc(fp);
			/*ENTRY_FLAG 0x0b - File attributes*/
			Shift_Offset(ENTRY_FLAG+offset+0x20*(entr_locat-1));	
		}while((flag = fgetc(fp)) == 0x0f);
		printf("entr_locat: %d\n",entr_locat);
		
		/*Get the first entry(1st index)->calloc*/
		if((*cnt_entr_sh == 0) && (temp != 0)){
			//Get data for the entry
			entr_sh = (Entry_Short*)calloc(1,sizeof(Entry_Short));
			//cnt_entr_sh: The Entry Index in the Struct Array
			getShortEntry(&entr_sh[*cnt_entr_sh], offset+0x20*(entr_locat-1));
			(*cnt_entr_sh)++;
		}
		
		/*Get entry starting from second index->realloc*/
		else if(temp!=0){
			entr_sh=(Entry_Short*)realloc(entr_sh,sizeof(Entry_Short));
			getShortEntry(&entr_sh[*cnt_entr_sh], offset+0x20*(entr_locat-1));
			(*cnt_entr_sh)++;
		}
	}
	return entr_sh;
}

void getShortEntry(Entry_Short *entr_sh, int offset){
	/*printf("getShortEntry\n");*/
	int i;
	Shift_Offset(offset+ENTRY_FILE_NAME);  
	memset(entr_sh->name,0,strlen(entr_sh->name));
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++){
		entr_sh->name[i]=fgetc(fp); 
	}
	Shift_Offset(ENTRY_FILE_NAME_EXT+offset);
	memset(entr_sh->ext,0,strlen(entr_sh->ext));
	for(i=0;i<ENTRY_FILE_NAME_EXT_BYTE;i++){
		entr_sh->ext[i]=fgetc(fp);
	}
	Shift_Offset(ENTRY_FILE_ATTR+offset);
	for(i=0;i<ENTRY_FILE_ATTR_BYTE;i++){
		entr_sh->attr=fgetc(fp);
	}
	Shift_Offset(ENTRY_START_CLU_HIG+offset);
	for(i=0;i<ENTRY_START_CLU_HIG_BYTE;i++){
		entr_sh->first_clus_hig+=(fgetc(fp)<<(8*i));
	}
	entr_sh->first_clus_low=0;
	Shift_Offset(ENTRY_START_CLU_LOW+offset);
	for(i=0;i<ENTRY_START_CLU_LOW_BYTE;i++){
		entr_sh->first_clus_low+=(fgetc(fp)<<(8*i));	
	}	
}

void displayEntryShort(Entry_Short *entr_sh,int cnt_entr_sh,int i){
	printf("\ndisplayEntryShort\n");
	for(i; i<cnt_entr_sh; i++){
		printf("\nFile dir %d\n",i+1);
		printf("Name:");
		print_Str(&entr_sh[i].name[0],ENTRY_FILE_NAME_BYTE);
		print_Str(&entr_sh[i].ext[0],ENTRY_FILE_NAME_EXT_BYTE);
		printf("\n");
		//printf("%d\n",entr_sh[i].attr);
		//printf("start cluster:%d\n",entr_sh[i].first_clus_low);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
/* This funstion get value of FAT entry*/
void checkFile(Entry_Short **entr_sh, int *cnt_entr_sh, int i){	
	printf("\ncheckFile\n");
	int offset, sav_cnt_entr_old;
	 
	//Scan through all ROOT ENTRY 
	for(i; i<(*cnt_entr_sh); i++){
		//IF ENTRY IS FOLDER
		int check = ((entr_sh[0]+i) -> attr==0x10);
		if(check){	
			//locat_clus: first_clus_low - 0x1a 
			int locat_clus = (entr_sh[0]+i) -> first_clus_low;
			printf("locat_clus: %x\n");
			do{
				//offset: subfile location
				offset = ((locat_clus-2)+0x21)*0x200+32*2;
				printf("#Offset: %d\n",offset);
				//save old cnt_entr=7
				sav_cnt_entr_old =* cnt_entr_sh;
				printf("Old: %d\n",sav_cnt_entr_old);
				//push to entr_sh, cnt_entr increase
				readEntrInClus(*entr_sh, offset, cnt_entr_sh);
				///FAT12 
				locat_clus = GetFatValue12(locat_clus);
			}
			while(locat_clus != FAT12_EOF);///FAT12 
			
			displayEntryShort(entr_sh[0],*cnt_entr_sh,sav_cnt_entr_old);//display new add list
		}	
	}
}

void readEntrInClus(Entry_Short *entr_sh, int offset, int *cnt_entr_sh){
	printf("\nreadEntrInClus\n");
	int entr_locat = 0, flag;
	int temp = 1;
	//16 entry in 1 sector, 1 sector in 1 cluster 
	while(entr_locat < (16*Boot->sec_per_clus) && temp!=0){
		entr_locat++;
		printf("entr_locat: %d\n",entr_locat);
		Shift_Offset(offset+0x20*(entr_locat-1));
		temp = fgetc(fp);
		if(temp!=0){
			entr_sh=(Entry_Short*)realloc(entr_sh, sizeof(Entry_Short));
			getShortEntry(&entr_sh[*cnt_entr_sh], offset+0x20*(entr_locat-1));
			(*cnt_entr_sh)++;
		}
	}
}

uint16_t GetFatValue12(uint16_t PrsClus) /*for FAT12*/{
	printf("\nGetFatValue12\n");
    unsigned char DataOfIndex[2]; /* array save 2 hex of FAT*/
    uint16_t NewVal;
    uint16_t HIG;
    uint16_t LOW;
    
    //fseek (fp , 512 + (PrsClus * 3) / 2 , SEEK_SET);
	Shift_Offset(512 + (PrsClus * 3) / 2);
    /* get data into array */
    DataOfIndex[0] = fgetc(fp);
    DataOfIndex[1] = fgetc(fp);
    HIG = DataOfIndex[1];
    LOW = DataOfIndex[0];
	//printf("\n%x	%x\n",DataOfIndex[1],DataOfIndex[0]);
    if (PrsClus % 2 == 0){
        NewVal = (LOW | ((HIG & 0xF) << 8));
    }else{
        NewVal = ((HIG << 4) | (LOW >> 4));
    }
    return NewVal;
}
 
  
void readData(){
	printf("readData\n");
	//1. Boot
	Boot = readBootSector();
	displayBoot(*Boot);
	
	int cnt_entr_sh = 0, sav_cnt_entr_old = 0;
	
	//root directory location: 0x0e+0x16*0x200
	int offset = (Boot->resv_sec_cnt + Boot->sec_per_FAT*Boot->num_FAT)*0x200;
	
	//number of clusters - entries in root
	//clus in root (entry) is 16 Byte
	int cnt_clus_root = Boot->root_entr_cnt*32/512;
	
	//1. Root
	//offset: 2600, cnt_entr_sh=7
	//readEntryShort(int offset,int size_root,int *cnt_entr_sh)
	Entry_Short *entr_sh = readEntryShort(offset, cnt_clus_root*16, &cnt_entr_sh);
	
	//displayEntryShort(Entry_Short *entr_sh,int cnt_entr_sh,int i)
	displayEntryShort(&entr_sh[0], cnt_entr_sh, 0);
	
	//checkFile(Entry_Short **entr_sh,int *cnt_entr_sh,int i)
	checkFile(&entr_sh, &cnt_entr_sh, 0);	
		
	printf("////////////*****__*****///////////");
	printf("\nSelect the file to display data by number on the screen:");	
	int temp;
	scanf("%d",&temp);
	int status = getDataFile(&entr_sh[0],temp-1);
}


int getDataFile(Entry_Short *entr_sh,int index){
	printf("getDataFile\n");
	printf("\n\nName:");
	print_Str(&entr_sh[index].name[0],ENTRY_FILE_NAME_BYTE);
	print_Str(&entr_sh[index].ext[0],ENTRY_FILE_NAME_EXT_BYTE);
	printf("\n");
	
	int check=((entr_sh[index]).attr == 0x10);
	if(check){
		printf("--->folder has been opened!\n");
		return 0;
	}
	int locat_clus=(entr_sh[index]).first_clus_low;
	do{
		int i;
		int offset=((locat_clus-2)+0x21)*0x200;//data location
		printf("\nofset:%d\n",offset);
		Shift_Offset(offset);
		for(i=0;i<(512*Boot->sec_per_clus);i++){
			printf("%c",fgetc(fp));
		}
		locat_clus=GetFatValue12(locat_clus);///FAT12******************************************
		//printf("\nlocat_clus:%p\n",locat_clus);
	}while(locat_clus!=FAT12_EOF);///FAT12*****************************************************
	return 1;
}

#endif
