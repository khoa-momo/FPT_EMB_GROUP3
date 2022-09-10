#ifndef	__FAT_SYSTEM_C__
#define	__FAT_SYSTEM_C__
#include"FAT_system.h"


//void getChar(char *temp,int byte){
//	int i;
//	for(i=0;i<byte;i++){
//		temp=fgetc(fp);
//	}
//}

void print_Str(char str[],int size )
{
	int i;
	for(i=0;i<size;i++)
	{
		printf("%c",str[i]);
	}
}

///////////Boot_sector////////////////
void getBootSector(Boot_Sector *Boot)
{
	int i;
	Shift_Offset(BOOT_BYTE_PER_SEC);
	for(i=0;i<BOOT_BYTE_PER_SEC_BYTE;i++){
		Boot->byte_per_sec+=(fgetc(fp)<<(8*i));
	}
	Shift_Offset(BOOT_SEC_PER_CLUS);
//	getChar(Boot->sec_per_clus,BOOT_SEC_PER_CLUS_BYTE);
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
	printf("\nByte_per_sec:%d\n",Boot.byte_per_sec);
	printf("Sec_per_clus:%d\n",Boot.sec_per_clus);
	printf("Resv_sec_cnt:%d\n",Boot.resv_sec_cnt);
	printf("Num_FAT:%d\n",Boot.num_FAT);
	printf("Root_entr_cnt:%d\n",Boot.root_entr_cnt);
	printf("Sec_per_FAT:%d\n",Boot.sec_per_FAT);
	printf("FAT_type:");
	print_Str(Boot.FAT_type,BOOT_FAT_TYPE_BYTE);
	printf("\n");
	//printf("%mark:s\n",Boot.mark);
}////////////////////////////////////////////

//////////////////entry short////////////////
void getShortEntry(Entry_Short *entr_sh,int offset){
	int i;
	Shift_Offset(offset+ENTRY_FILE_NAME);
	//printf("i:%d %s %d\n",i,entr_sh->name,strlen(entr_sh->name));
	
	memset(entr_sh->name,0,strlen(entr_sh->name));
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++){
		entr_sh->name[i]=fgetc(fp);
		//printf("i:%d %s %d\n",i,entr_sh->name,strlen(entr_sh->name));
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

Boot_Sector *readBootSector()
{
	Boot_Sector *Boot=(Boot_Sector*)calloc(1,sizeof(Boot_Sector));
	getBootSector(Boot);
	return Boot;
}

Entry_Short *readEntryShort(int offset,int size_root,int *cnt_entr_sh)
{
	int entr_locat=0,flag;
	Entry_Short*entr_sh;
	int temp=1;
	while(entr_locat<size_root&&temp!=0){
		do{
			entr_locat++;
			Shift_Offset(offset+0x20*(entr_locat-1));
			temp=fgetc(fp);
			Shift_Offset(ENTRY_FLAG+offset+0x20*(entr_locat-1));	
		}while((flag=fgetc(fp))==0x0f);
		if((*cnt_entr_sh==0)&&(temp!=0)){
			entr_sh=(Entry_Short*)calloc(1,sizeof(Entry_Short));
			getShortEntry(&entr_sh[*cnt_entr_sh],offset+0x20*(entr_locat-1));
			(*cnt_entr_sh)++;
		}
		else if(temp!=0){
			entr_sh=(Entry_Short*)realloc(entr_sh,sizeof(Entry_Short));
			getShortEntry(&entr_sh[*cnt_entr_sh],offset+0x20*(entr_locat-1));
			(*cnt_entr_sh)++;
		}
	}
	return entr_sh;
}
void readEntrInClus(Entry_Short *entr_sh,int offset, int *cnt_entr_sh)
{
	int entr_locat=0,flag;
	int temp=1;
	while(entr_locat<16&&temp!=0){//16 entry in 1 cluster ****
		entr_locat++;
		Shift_Offset(offset+0x20*(entr_locat-1));
		temp=fgetc(fp);
		if(temp!=0){
			entr_sh=(Entry_Short*)realloc(entr_sh,sizeof(Entry_Short));
			getShortEntry(&entr_sh[*cnt_entr_sh],offset+0x20*(entr_locat-1));
			(*cnt_entr_sh)++;
		}
	}

}

void displayEntryShort(Entry_Short *entr_sh,int cnt_entr_sh,int i)
{
	for(i;i<cnt_entr_sh;i++){
		printf("\nFile dir %d\n",i+1);
		printf("Name:");
		print_Str(&entr_sh[i].name[0],ENTRY_FILE_NAME_BYTE);
		print_Str(&entr_sh[i].ext[0],ENTRY_FILE_NAME_EXT_BYTE);
		printf("\n");
		//printf("%d\n",entr_sh[i].attr);
		//printf("start cluster:%d\n",entr_sh[i].first_clus_low);
	}
}

void readData(){
	int cnt_entr_sh=0, sav_cnt_entr_old=0;
	int offset=0x2600;
	Entry_Short *entr_sh=readEntryShort(offset,14*16,&cnt_entr_sh);//cnt_entr_sh=7
	
//	printf("add:%d\n",&entr_sh[0]);
//	printf("add:%d\n",&entr_sh[1]);
//	printf("add:%d\n",&entr_sh[2]);
//	printf("add:%d\n",&entr_sh[3]);
	
	displayEntryShort(&entr_sh[0],cnt_entr_sh,0);
	checkFile(&entr_sh,&cnt_entr_sh,0);		
}


void checkFile(Entry_Short **entr_sh,int *cnt_entr_sh,int i)
{
	
	//Boot_Sector *Boot=readBootSector();

	//int temp=Boot->resv_sec_cnt+Boot->sec_per_FAT*Boot->num_FAT+Boot->root_entr_cnt*32/Boot->byte_per_sec;
	//printf("temp:%d\n",temp);
	
	
	int offset,sav_cnt_entr_old;
	for(i;i<(*cnt_entr_sh);i++){
		//displayEntryShort(entr_sh[0],*cnt_entr_sh,i);

	
		int check=((entr_sh[0]+i)->attr==0x10);
		printf("\n\ncheck:%d\n",check);
		if(check)
		{		
			offset=(((entr_sh[0]+i)->first_clus_low-2)+0x21)*0x200+32*2;
			printf("\noffset:%d\n",offset);
			sav_cnt_entr_old=*cnt_entr_sh;//save old cnt_entr=7
			printf("old:%d\n",sav_cnt_entr_old);
			readEntrInClus(*entr_sh,offset,cnt_entr_sh);//push to entr_sh, cnt_entr increase
			displayEntryShort(entr_sh[0],*cnt_entr_sh,sav_cnt_entr_old);
		}	

	}
}

int checkChoose(Entry_Short *entr_sh,int cnt_entr_sh){
	int i;
	for(i=0;i<cnt_entr_sh;i++){
		
	}
}



#endif
