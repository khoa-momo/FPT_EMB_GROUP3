#ifndef	__FAT_SYSTEM_C__
#define	__FAT_SYSTEM_C__
#include"FAT_system.h"

/*--------------------------- I/ BOOT SECTOR - START ---------------------------*/
void getBootSector(Boot_Sector *Boot){				//Boot_sector//
	int i;
	/*1.FAT*/
	/*0x36 - char*/
	Shift_Offset(BOOT_FAT_TYPE);
	for(i=0;i<BOOT_FAT_TYPE_BYTE;i++){
		Boot->FAT_type[i]=fgetc(fp);	
	}
	/*0x10 - char*/
	Shift_Offset(BOOT_NUM_FAT);
	for(i=0;i<BOOT_NUM_FAT_BYTE;i++){
		Boot->num_FAT=fgetc(fp);
	}
	/*0x16 - int*/
	Shift_Offset(BOOT_SEC_PER_FAT);
	for(i=0;i<BOOT_SEC_PER_FAT_BYTE;i++){
		Boot->sec_per_FAT+=(fgetc(fp)<<(8*i));
	}
	/*0x10 - int*/
	Shift_Offset(BOOT_RSV_SEC_CNT);
	for(i=0;i<BOOT_RSV_SEC_CNT_BYTE;i++){
		Boot->resv_sec_cnt+=(fgetc(fp)<<(8*i));
	}
	/*2.Root*/
	/*0x11 -int*/
	Shift_Offset(BOOT_ROOT_ENT_CNT);
	for(i=0;i<BOOT_ROOT_ENT_CNT_BYTE;i++){
		Boot->root_entr_cnt+=(fgetc(fp)<<(8*i));
	}
	/*3.Data*/
	/*0x0b - BLock/Sec Size - 512B - int*/
	Shift_Offset(BOOT_BYTE_PER_SEC);
	for(i=0;i<BOOT_BYTE_PER_SEC_BYTE;i++){
		Boot->byte_per_sec+=(fgetc(fp)<<(8*i));
	} 
	/*0x0d - block/sector per cluster - char*/
	Shift_Offset(BOOT_SEC_PER_CLUS); 
	for(i=0;i<BOOT_SEC_PER_CLUS_BYTE;i++){
		Boot->sec_per_clus=fgetc(fp);
	}
}

void displayBoot(Boot_Sector Boot){					//Boot_sector//
	printf("\nByte_per_sec:%d\n",Boot.byte_per_sec);
	printf("Sec_per_clus:%d\n",Boot.sec_per_clus);
	printf("Resv_sec_cnt:%d\n",Boot.resv_sec_cnt);
	printf("Num_FAT:%d\n",Boot.num_FAT);
	printf("Root_entr_cnt:%d\n",Boot.root_entr_cnt);
	printf("Sec_per_FAT:%d\n",Boot.sec_per_FAT);
	printf("FAT_type:");
	print_Str(Boot.FAT_type,BOOT_FAT_TYPE_BYTE);
	printf("\n"); 
} 

Boot_Sector *readBootSector(){						//Boot_sector//
	Boot_Sector *Boot=(Boot_Sector*)calloc(1,sizeof(Boot_Sector));
	getBootSector(Boot);
	return Boot;
}
/*------------------------------------------------------------------------------*/



/*--------------------------- II/ FAT - START ---------------------------*/
/* This funstion get value of FAT entry*/
uint16_t GetFatValue12(uint16_t PrsClus)/*for FAT12*/{
	printf("\n### GetFatValue12 ###\n");
    unsigned char DataOfIndex[2]; /* array save 2 hex of FAT*/
    uint16_t NewVal;
    uint16_t HIG;
    uint16_t LOW;
    
	Shift_Offset(512 + (PrsClus * 3) / 2);
    /* get data into array */
    DataOfIndex[0] = fgetc(fp);
    DataOfIndex[1] = fgetc(fp);
    HIG = DataOfIndex[1];
    LOW = DataOfIndex[0];
	//printf("\n%x	%x\n",DataOfIndex[1],DataOfIndex[0]);
    if (PrsClus % 2 == 0){
        NewVal = (LOW | ((HIG & 0xF) << 8));
    }
    else{
        NewVal = ((HIG << 4) | (LOW >> 4));
    }
    return NewVal;
}
/*-----------------------------------------------------------------------*/



/*--------------------------- III/ ROOT/ENTRY - START ---------------------------*/
/*Get entry atr*/
void getShortEntry(Entry_Short *entr_sh, int offset){							//entry short//
	int i; 
	//printf("i:%d %s %d\n",i,entr_sh->name,strlen(entr_sh->name));
	/*0x00 - File name (8B)*/
	Shift_Offset(offset+ENTRY_FILE_NAME);
	memset(entr_sh->name,0,strlen(entr_sh->name));
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++){
		entr_sh->name[i]=fgetc(fp); 
	}
	/*0x08 - File extension (3B)*/
	Shift_Offset(ENTRY_FILE_NAME_EXT+offset);
	memset(entr_sh->ext,0,strlen(entr_sh->ext));
	for(i=0;i<ENTRY_FILE_NAME_EXT_BYTE;i++){
		entr_sh->ext[i]=fgetc(fp);
	}
	/*0x0b - File attribute (1B)*/
	Shift_Offset(ENTRY_FILE_ATTR+offset);
	for(i=0;i<ENTRY_FILE_ATTR_BYTE;i++){
		entr_sh->attr=fgetc(fp);
	}
	/*0x14 - Start cluster number of file - HIGH (2B)*/
	Shift_Offset(ENTRY_START_CLU_HIG+offset);
	for(i=0;i<ENTRY_START_CLU_HIG_BYTE;i++){
		entr_sh->first_clus_hig+=(fgetc(fp)<<(8*i));
	} 
	/*0x16 - Time last updated (2B)*/
	Shift_Offset(ENTRY_TIME_CRE_UPD+offset);
	for(i=0;i<ENTRY_TIME_CRE_UPD_BYTE;i++){
		entr_sh->timeUpdate+=(fgetc(fp)<<(8*i));
	}
	/*0x16 - Time last updated (2B)*/
	Shift_Offset(ENTRY_DATE_CRE_UPD+offset);
	for(i=0;i<ENTRY_DATE_CRE_UPD_BYTE;i++){
		entr_sh->dateUpdate+=(fgetc(fp)<<(8*i));
	} 
	/*0x1a - Start cluster number of file - LOW (2B)*/
	entr_sh->first_clus_low=0;
	Shift_Offset(ENTRY_START_CLU_LOW+offset);
	for(i=0;i<ENTRY_START_CLU_LOW_BYTE;i++){
		entr_sh->first_clus_low+=(fgetc(fp)<<(8*i));	
	}	
}

void displayEntryShort(Entry_Short *entr_sh, int cnt_entr_sh, int i){			//entry short//
	for(i; i<cnt_entr_sh; i++){
		printf("\n------------------------------------ File dir %d ------------------------------------\n", i);
		//printf("%d ", entr_sh[i].attr);
		if(entr_sh[i].attr == 0x0){
			printf("File Name: ");
		} 
		else if(entr_sh[i].attr == 0x10){
			printf("Folder Name: ");
		}
		print_Str(&entr_sh[i].name[0],ENTRY_FILE_NAME_BYTE);
		print_Str(&entr_sh[i].ext[0],ENTRY_FILE_NAME_EXT_BYTE); 
		
		printf("\t\tTime Update: "); readTime(entr_sh[i].timeUpdate); 
        printf("\n\t\t\t\tDate Update: "); readDate(entr_sh[i].dateUpdate);
        
		printf("\n");
		//printf("%atribute:d\n",entr_sh[i].attr);
		//printf("start cluster:%d\n",entr_sh[i].first_clus_low);
	}
}

/*Scan entry*/
void countEntryShort(int offset, int *cnt_entr_sh){								//entry short//
	printf("\n##### CountEntryShort #####\n");
	printf("Boot->root_entr_cnt: %d\n", Boot->root_entr_cnt);
	//printf("cnt:%d\n",*cnt_entr_sh);
	//printf("offset:%d\n",offset); 
	*cnt_entr_sh = 0;
	//number of clusters in root
	//int cnt_clus_root = Boot->root_entr_cnt*32/512;
	int entr_locat = 0, flag;
	int temp = 1;
	//printf("check:%d\n",entr_locat<(cnt_clus_root*16)&&temp!=0);
	//printf("cnt_clus_root*16: %d\n", cnt_clus_root*16);
	while(entr_locat<Boot->root_entr_cnt && temp!=0){
		do{
			entr_locat++;
			Shift_Offset(offset+0x20*(entr_locat-1));
			temp=fgetc(fp);
			Shift_Offset(ENTRY_FLAG+offset+0x20*(entr_locat-1));	
		}while((flag=fgetc(fp))==0x0f);
		if(temp!=0){
			(*cnt_entr_sh)++;
			//printf("cnt:%d\n",*cnt_entr_sh); 
		}
	}
	//printf("cnt:%d\n",*cnt_entr_sh);
}

Entry_Short *readEntryShort(int offset, int *cnt_entr_sh){						//entry short//			 
	//printf("\nReadEntryShort\n");
	printf("\n##### ReadEntryShort #####\n");
	Entry_Short *entr_sh;
	//number of clusters in root
	int cnt_clus_root = Boot->root_entr_cnt*32/512;
	//entry index
	int index_entr = 0;
	//entr_locat: entry number th
	int entr_locat = 0, flag;
	
	//offset:0x2600	
	countEntryShort(offset, cnt_entr_sh);
	if(*cnt_entr_sh>0){
		//Declare Entry as struct
		entr_sh = (Entry_Short*)malloc((*cnt_entr_sh)*sizeof(Entry_Short));
		int temp = 1;
		while(entr_locat<(cnt_clus_root*16) && temp!=0){
			/*Find Main Entry, Pass all Sub Entry*/
			do{
				entr_locat++;
				printf("offset+0x20*(entr_locat-1): %x\n",offset+0x20*(entr_locat-1));
				//entr_locat-1 so the 1st index of entr_locat can start from 0
				//Shift 32B from root start
				Shift_Offset(offset+0x20*(entr_locat-1));
				//temp: Get 1st Byte of current Entry
				temp = fgetc(fp);
				/*ENTRY_FLAG 0x0b - File attributes*/
				Shift_Offset(ENTRY_FLAG+offset+0x20*(entr_locat-1));	
			}while((flag = fgetc(fp)) == 0x0f);
			/*0x0f -> Sub Entry (Long Entry)*/
			if(temp!=0){
				printf("++++Add new entry++++\n");
				getShortEntry(&entr_sh[index_entr], offset+0x20*(entr_locat-1));
				index_entr++;
			}
		}
	}
	else{
		printf("Root Entry Is Empty\n");
		entr_sh = NULL;
	}
	//Return Array of Struct Entry
	return entr_sh;
}

void countEntryShortClus(int offset, int *cnt_entr_sh, int locat_clus){			//entry short//
	printf("\n##### CountEntryShortClus #####\n");
	*cnt_entr_sh = 0;
	int entr_locat = 0, flag;
	int temp = 1;
	do{
		while(entr_locat<(16) && temp!=0){
			do{
				entr_locat++;
				Shift_Offset(offset+0x20*(entr_locat-1));
				temp = fgetc(fp);
				Shift_Offset(ENTRY_FLAG+offset+0x20*(entr_locat-1));	
			}while((flag=fgetc(fp)) == 0x0f);
			if(temp!=0){
				(*cnt_entr_sh)++;	
			}
		}
		//get file data cluster from FAT - FAT12
		locat_clus = GetFatValue12(locat_clus);
	}while(locat_clus != FAT12_EOF);///FAT12 
}

Entry_Short *readEntrInClus(int offset, int *cnt_entr_sh,int locat_clus){		//entry short//
	printf("\n##### ReadEntrInClus #####\n");
	int index_entr=0;
	int cnt_clus_root = Boot->root_entr_cnt*32/512;//number of clusters in root
	int entr_locat=0, flag;
	
	Entry_Short *entr_sh;
	countEntryShortClus(offset, cnt_entr_sh, locat_clus);
	
	if(*cnt_entr_sh>0){
		entr_sh = (Entry_Short*)malloc((*cnt_entr_sh)*sizeof(Entry_Short));		
		do{
			int temp = 1;
			//16 entry in 1 sector, 1 sector in 1 cluster 
			while(entr_locat<(16)&&temp!=0){
				entr_locat++;
				printf("entr_locat: %d\n", entr_locat);
				Shift_Offset(offset+0x20*(entr_locat-1));
				temp = fgetc(fp);
				
				if(temp!=0){
					printf("++++Add new entry++++\n");
					getShortEntry(&entr_sh[index_entr],offset+0x20*(entr_locat-1));
					index_entr++;
				}
			}
			//get file data cluster from FAT - FAT12
			locat_clus = GetFatValue12(locat_clus);
		}while(locat_clus!=FAT12_EOF);///FAT12 
	}else{
		printf("File is empty\n");
		entr_sh = NULL;
	}	
	return entr_sh;
}
/*-------------------------------------------------------------------------*/



/*--------------------------- IV/ DATA - START ---------------------------*/
int getDataFile(Entry_Short *entr_sh, int index){				//DATA//
	printf("\n### GetDataFile ###\n");
	printf("\n\nName:");
	print_Str(&entr_sh[index].name[0],ENTRY_FILE_NAME_BYTE);
	print_Str(&entr_sh[index].ext[0],ENTRY_FILE_NAME_EXT_BYTE);
	printf("\n");
	
	int locat_clus = (entr_sh[index]).first_clus_low;
	do{
		int i;
		int offset = ((locat_clus-2)+bef_data_sector)*Boot->byte_per_sec;//data location
		Shift_Offset(offset);
		for(i=0;i<(512*Boot->sec_per_clus);i++){
			printf("%c",fgetc(fp));
		}
		locat_clus = GetFatValue12(locat_clus);///FAT12 
		printf("\n~~~~~~~~~ Locat_clus: %x ~~~~~~~~~\n",locat_clus);
	}while(locat_clus!=FAT12_EOF);///FAT12 
	return 1;
}
/*---------------------------------------------------------------------*/


/*--------------------------- PRINTING ---------------------------*/
void print_Str(char str[], int size){
	int i;
	for(i=0;i<size;i++){
		printf("%c",str[i]);
	}
}

void readDate(int raw_date){
	int mask_day = 0x001f;
	int mask_month = 0x01e0;
	int mask_year = 0xfe00;
	
	int day = (raw_date & mask_day);
	int moth = (raw_date & mask_month)>>5;
	int year = ((raw_date & mask_year)>>9)+1980;
	 
	printf("Date(YYYY/MM/DD): %d/%d/%d   ",year,moth,day); 
}

void readTime(int raw_time){ 
	int mask_sec = 0x001f;
	int mask_min = 0x07e0;
	int mask_hr = 0xf800;
	
	int sec = (raw_time & mask_sec);
	int min = (raw_time & mask_min)>>5;
	int hr = ((raw_time & mask_hr)>11)*2;
	
	printf("Time(hh:mm:ss): %d:%d:%d   ",hr,min,sec);
}

int offset_clus(int first_clus_low){
	return ((first_clus_low-2)+0x21)*0x200+32*2;
}

void main_offset(){
	//Root_Sectors = (SF*NF)+BF;/19
	//Boot->num_FAT, Boot->sec_per_FAT, Boot->resv_sec_cnt
	Root_offset = ((Boot->sec_per_FAT * Boot->num_FAT) + Boot->resv_sec_cnt) * Boot->byte_per_sec;
	bef_data_sector = ((Boot->sec_per_FAT * Boot->num_FAT) + Boot->resv_sec_cnt)+(Boot->root_entr_cnt*32/Boot->byte_per_sec);//33 
	printf("bef_data_sector: %d\n",bef_data_sector);
	 
}

//checking
void printLinkedList(Node *head){
 	while(head != NULL) {
 		printf("\nLL-name: %s",(head->entr_sh).name);
		//print_Str(&(), ENTRY_FILE_NAME_BYTE); 
 		printf("\n");
      	head = head->next;
 	}	
}
 
#endif
