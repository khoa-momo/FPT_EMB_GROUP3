#ifndef	__FAT_SYSTEM_C__
#define	__FAT_SYSTEM_C__
#include"FAT_system.h"

/*------------------------------- I/ LINK LIST - START ---------------------------------------*/
void initList(){						//create list								/*linklist*/
	head = NULL;
}
bool isEmpty(){							//check list empty							/*linklist*/
	if(head==NULL){
		return true;
	}
	return false;
}
void createNode(Entry_Short *entr_sh){	//create node with data						/*linklist*/	
	Node *node = (Node*)calloc(1,sizeof(Node));
	//1.Add in an empty node
	addNodeToList(node);
	int i;
	//2.Fill attr: name & first_clus_low to that node
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++){
		(node->entr_sh).name[i]=entr_sh[0].name[i];
	}
	(node->entr_sh).first_clus_low=entr_sh[0].first_clus_low;
	printLinkedList(head);
}
void addNodeToList(Node *node){			//add First Node to list					/*linklist*/		
	if(isEmpty()){
		head=node;
		head->next=NULL;
	}
	else{
		node->next=head;
		head=node;
	}
}
void removeNode(){						//delete node								/*linklist*/
	Node *last_Node = head;
	//List empty
	if(isEmpty()){
	}
	//List has 1 Node only
	else if(head->next==NULL){
		free(last_Node);
		head=NULL;		
	}
	//List has >1 Node only
	else{	
		head=head->next;	
	}
	printLinkedList(head);
}
void printLinkedList(Node *head){		//print list								/*linklist*/
 	if(head != NULL) {
 		printf("\nFile-opening: %s",(head->entr_sh).name); 
 		printf("\n");
      	//head = head->next;
 	}	
}
/*---------------------------------- LINK LIST - END -----------------------------------------*/

/*--------------------------- II/ BOOT SECTOR - START ---------------------------*/
void getBootSector(Boot_Sector *Boot){								/*Boot_sector*/
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
void displayBoot(Boot_Sector Boot){									/*Boot_sector*/
	printf("------------------ CALL BOOT ------------------\n");
	printf("---------   ");
		print_Str(Boot.FAT_type,BOOT_FAT_TYPE_BYTE);
	printf("---------\n");
	printf("Byte_per_sec:\t%d\n",Boot.byte_per_sec);
	printf("Sec_per_clus:\t%d\n",Boot.sec_per_clus);
	printf("Resv_sec_cnt:\t%d\n",Boot.resv_sec_cnt);
	printf("Num_FAT:\t%d\n",Boot.num_FAT);
	printf("Root_entr_cnt:\t%d\n",Boot.root_entr_cnt);
	printf("Sec_per_FAT:\t%d\n",Boot.sec_per_FAT);
	printf("----------------------------\n");
	printf("----------------------------------------------\n");
} 
Boot_Sector *readBootSector(){										/*Boot_sector*/
	Boot_Sector *Boot=(Boot_Sector*)calloc(1,sizeof(Boot_Sector));
	getBootSector(Boot);
	return Boot;
}
/*---------------------------- BOOT SECTOR - END --------------------------------*/


/*--------------------------- III/ FAT - START ---------------------------*/
/* This funstion get value of FAT entry*/
uint16_t GetFatValue12(uint16_t PrsClus)/*for FAT12*/{
	//printf("\n### GetFatValue12 ###\n");
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
uint16_t GetFatValue16( uint16_t PrsClus) /*for FAT16*/
{
    unsigned char DataOfIndex[2]; /* array save 2 hex of FAT*/
    uint16_t NewVal;
    uint16_t HIG;
    uint16_t LOW;

	Shift_Offset(512 + (PrsClus * 2));

     /* get data into array */
    DataOfIndex[0] = fgetc(fp);
    DataOfIndex[1] = fgetc(fp);

    HIG = DataOfIndex[1];
    LOW = DataOfIndex[0];

    NewVal = (HIG << 8) | LOW;
    return NewVal;
}
/*--------------------------- FAT - END ---------------------------------*/


/*--------------------------- IV/ ROOT/ENTRY - START -----------------------------------------*/
/*Get entry atr*/
void getShortEntry(Entry_Short *entr_sh, int offset){							/*entry short*/
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
void displayEntryShort(Entry_Short *entr_sh, int cnt_entr_sh, int i){			/*entry short*/
	printf("\n\n------------------------------------ FILE DIRECTORY ------------------------------------\n");
	for(i; i<cnt_entr_sh; i++){
		printf("\n------------------------------------ File Direct %d ------------------------------------\n", i);
		if(entr_sh[i].attr == 0x0){
			printf("File Name:\t ");
		} 
		else if(entr_sh[i].attr == 0x10){
			printf("Folder Name:\t ");
		}
		print_Str(&entr_sh[i].name[0],ENTRY_FILE_NAME_BYTE);
		print_Str(&entr_sh[i].ext[0],ENTRY_FILE_NAME_EXT_BYTE); 
		
		printf("\t\tTime Update: "); readTime(entr_sh[i].timeUpdate); 
        printf("\n\t\t\t\t\tDate Update: "); readDate(entr_sh[i].dateUpdate);
	}
}
/*Scan entry*/
void countEntryShort(int offset, int *cnt_entr_sh){								/*entry short*/
	*cnt_entr_sh = 0;
	//number of clusters in root
	//int cnt_clus_root = Boot->root_entr_cnt*32/512;
	int entr_locat = 0, flag;
	int temp = 1;

	while(entr_locat<Boot->root_entr_cnt && temp!=0){
		do{
			entr_locat++;
			Shift_Offset(offset+0x20*(entr_locat-1));
			temp=fgetc(fp);
			Shift_Offset(ENTRY_FLAG+offset+0x20*(entr_locat-1));	
		}while((flag=fgetc(fp))==0x0f);
		if(temp!=0){
			(*cnt_entr_sh)++; 
		}
	}
}
Entry_Short *readEntryShort(int offset, int *cnt_entr_sh){						/*entry short*/		 
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
				getShortEntry(&entr_sh[index_entr], offset+0x20*(entr_locat-1));
				index_entr++;
			}
		}
	}
	else{
		printf("File Is Empty\n");
		entr_sh = NULL;
	}
	//Return Array of Struct Entry
	return entr_sh;
}
void countEntryShortClus(int offset, int *cnt_entr_sh, int locat_clus){			/*entry short*/
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
Entry_Short *readEntrInClus(int offset, int *cnt_entr_sh,int locat_clus){		/*entry short*/
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
				Shift_Offset(offset+0x20*(entr_locat-1));
				temp = fgetc(fp);
				
				if(temp!=0){
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
/*-------------------------------------------------------------------------------------------*/


/*--------------------------- PRINTING ----------------------------*/
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
/*-----------------------------------------------------------------*/


int offset_clus(int first_clus_low){
	return ((first_clus_low-2)+0x21)*0x200+32*2;
}
void main_offset(){
	Root_offset = ((Boot->sec_per_FAT * Boot->num_FAT) + Boot->resv_sec_cnt) * Boot->byte_per_sec;
	bef_data_sector = ((Boot->sec_per_FAT * Boot->num_FAT) + Boot->resv_sec_cnt)+(Boot->root_entr_cnt*32/Boot->byte_per_sec);//33 	 
}
void prtByteFile(int n){
	int i;
	for(i=0;i<n;i++){
			printf("%c",fgetc(fp));
		}
}
 
#endif
