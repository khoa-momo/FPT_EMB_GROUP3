#ifndef	__FAT_SYSTEM_C__
#define	__FAT_SYSTEM_C__
#include"FAT_system.h"

///////////LINK LIST - start//////////////////////////////////////////////////////////
//create list
void initList(){											////////linklist////////
	head=NULL;
}
//check list empty
bool isEmpty(){												///////linklist///////
	return head==NULL;
}
//create node with data
void createNode(Entry_Short *entr_sh){						///////linklist/////
	Node* node=(Node*)malloc(sizeof(Node));
	addNodeToList(node);
	int i;
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++)
	{
		(node->entr_sh).name[i]=entr_sh[0].name[i];
	}
	(node->entr_sh).first_clus_low=entr_sh[0].first_clus_low;
}
//add Node to list
void addNodeToList(Node*node){								///////linklist///
	if(isEmpty()){
		printf("empty\n");
		head=node;
	}
	else{
		printf("not empty\n");
		node->next=head;
		//printf("AAAA\n");
		head=node;
		//3printf("AAAA\n");
	}
}
//delete node
void removeNode(){											/////linklist///
	Node*last_Node;
	last_Node=head;
	if(isEmpty()){
		printf("empty\n");
	}
	else if(last_Node->next==NULL){
		free(last_Node);
		head=NULL;		
	}
	else{
//		while((last_Node->next)->next!=NULL){
//			last_Node=last_Node->next;
//		}
//		free(last_Node->next);
//		last_Node->next=NULL;	
		head=head->next;	
	}
}

///////////LINK LIST - end////////////////////////////////////////////////

void print_Str(char str[],int size )
{
	int i;
	for(i=0;i<size;i++)
	{
		printf("%c",str[i]);
	}
}

//////////////Boot_sector-start///////////////////////////////////////////////////////
Boot_Sector *readBootSector()						///////////Boot_sector///////////
{
	Boot_Sector *Boot=(Boot_Sector*)calloc(1,sizeof(Boot_Sector));
	getBootSector(Boot);
	return Boot;
}
void getBootSector(Boot_Sector *Boot)				///////////Boot_sector/////////
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
void displayBoot(Boot_Sector Boot){					/////////Boot_sector/////////
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
}////////////////////////////////////////////////////
//////////////Boot_sector-end///////////////////////////////////////////////////

/* This funstion get value of FAT entry*/
uint16_t GetFatValue12(uint16_t PrsClus) /*for FAT12*/{
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
    if (PrsClus % 2 == 0)
    {
        NewVal = (LOW | ((HIG & 0xF) << 8));
    }
    else
    {
        NewVal = ((HIG << 4) | (LOW >> 4));
    }
    return NewVal;
}

//////////////////entry short - start//////////////////////////////////////////////////////////////
void countEntryShort(int offset,int *cnt_entr_sh){
	*cnt_entr_sh=0;
	//printf("cnt:%d\n",*cnt_entr_sh);
	//printf("offset:%d\n",offset);
	int cnt_clus_root=Boot->root_entr_cnt*32/512;//number of clusters in root
	int entr_locat=0,flag;
	int temp=1;
	//printf("check:%d\n",entr_locat<(cnt_clus_root*16)&&temp!=0);
	while(entr_locat<(cnt_clus_root*16)&&temp!=0){
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


Entry_Short * readEntryShort(int offset,int *cnt_entr_sh){	 /////entry short/////////
	printf("bbbb\n");

	int index_entr=0;
	int cnt_clus_root=Boot->root_entr_cnt*32/512;//number of clusters in root
	int entr_locat=0,flag;
	
	Entry_Short*entr_sh;
	countEntryShort(offset,cnt_entr_sh);
	
	if(*cnt_entr_sh>0){
		entr_sh=(Entry_Short*)malloc((*cnt_entr_sh)*sizeof(Entry_Short));
		int temp=1;
		while(entr_locat<(cnt_clus_root*16)&&temp!=0){
			do{
				entr_locat++;
				Shift_Offset(offset+0x20*(entr_locat-1));
				temp=fgetc(fp);
				Shift_Offset(ENTRY_FLAG+offset+0x20*(entr_locat-1));	
			}while((flag=fgetc(fp))==0x0f);
			if(temp!=0){
				printf("++++++++\n");
				getShortEntry(&entr_sh[index_entr],offset+0x20*(entr_locat-1));
				index_entr++;
			}
		}
	}else{
		printf("is empty\n");
		entr_sh=NULL;
	}
	return entr_sh;
}
void getShortEntry(Entry_Short *entr_sh,int offset){					 /////entry short////////
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
void displayEntryShort(Entry_Short *entr_sh,int cnt_entr_sh,int i){		 /////entry short///////
	for(i;i<cnt_entr_sh;i++){
		printf("\nFile dir %d\n",i+1);
		printf("Name:");
		print_Str(&entr_sh[i].name[0],ENTRY_FILE_NAME_BYTE);
		print_Str(&entr_sh[i].ext[0],ENTRY_FILE_NAME_EXT_BYTE);
		printf("\n");
		printf("%d\n",entr_sh[i].attr);
		printf("start cluster:%d\n",entr_sh[i].first_clus_low);
	}
}
void readEntrInClus(Entry_Short *entr_sh,int offset, int *cnt_entr_sh){	 ////entry short///////
	int entr_locat=0,flag;
	int temp=1;
	while(entr_locat<(16*Boot->sec_per_clus)&&temp!=0){//16 entry in 1 sector, 1 sector in 1 cluster 
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
/////entryNode
void countEntryShortClus(int offset,int *cnt_entr_sh,int locat_clus){
	*cnt_entr_sh=0;
	//printf("cnt:%d\n",*cnt_entr_sh);
	//printf("offset:%d\n",offset);
	int entr_locat=0,flag;
	int temp=1;

	do{
		while(entr_locat<(16)&&temp!=0){
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
		locat_clus=GetFatValue12(locat_clus);///FAT12******************************************
	}while(locat_clus!=FAT12_EOF);///FAT12******************************************
	
}
Entry_Short * readEntrInClusNode(int offset, int *cnt_entr_sh,int locat_clus)			////entry short//////
{
	int index_entr=0;
	int cnt_clus_root=Boot->root_entr_cnt*32/512;//number of clusters in root
	int entr_locat=0,flag;
	
	Entry_Short*entr_sh;
	countEntryShortClus(offset,cnt_entr_sh,locat_clus);
	
	if(*cnt_entr_sh>0){
		entr_sh=(Entry_Short*)malloc((*cnt_entr_sh)*sizeof(Entry_Short));		
		do{
			int temp=1;
			while(entr_locat<(16)&&temp!=0){//16 entry in 1 sector, 1 sector in 1 cluster 
				entr_locat++;
				Shift_Offset(offset+0x20*(entr_locat-1));
				temp=fgetc(fp);
				
				if(temp!=0){
				printf("++++++++\n");
				getShortEntry(&entr_sh[index_entr],offset+0x20*(entr_locat-1));
				index_entr++;
				}
			}
			locat_clus=GetFatValue12(locat_clus);///FAT12******************************************
		}while(locat_clus!=FAT12_EOF);///FAT12******************************************	
	}else{
		printf("is empty\n");
		entr_sh=NULL;
	}
		
	return entr_sh;
}
//////////////////entry short - end////////////////////////////////////////////////////////


/////////////////////////DATA -start////////////////////////////////////////////////////////////
void callBootSector(){											//////////////DATA/////////////
	Boot=readBootSector();
	displayBoot(*Boot);
}
void readData(){												//////////////DATA////////////
	callBootSector();
	
	int cnt_entr_sh=0;
	int offset=(Boot->resv_sec_cnt+Boot->sec_per_FAT*Boot->num_FAT)*0x200;//root directory location
	int cnt_clus_root=Boot->root_entr_cnt*32/512;//number of clusters in root
	
	Entry_Short *entr_sh=readEntryShort(offset,&cnt_entr_sh);//cnt_entr_sh=7
	displayEntryShort(&entr_sh[0],cnt_entr_sh,0);
	//checkFile(&entr_sh,&cnt_entr_sh);	
		
	printf("////////////*****__*****///////////");
	printf("\nSelect the file to display data by number on the screen:");	
	int temp;
	scanf("%d",&temp);
	temp--;
	int check=(entr_sh[temp].attr!=0x10);
	printf("check:%d\n",check);
	if(check){
		int status=getDataFile(&entr_sh[0],temp);
	}else{
		
		
	}	
	
}
void checkFile(Entry_Short **entr_sh,int *cnt_entr_sh)			//////////////DATA///////////
{	
	int offset,sav_cnt_entr_old,i;
	for(i=0;i<(*cnt_entr_sh);i++){
		int check=((entr_sh[0]+i)->attr==0x10);
		if(check)
		{	
			int locat_clus=(entr_sh[0]+i)->first_clus_low;
			do{
				offset=((locat_clus-2)+0x21)*0x200+32*2;//subfile location
				//printf("\noffset:%d\n",offset);
				sav_cnt_entr_old=*cnt_entr_sh;//save old cnt_entr=7
				//printf("old:%d\n",sav_cnt_entr_old);
				readEntrInClus(*entr_sh,offset,cnt_entr_sh);//push to entr_sh, cnt_entr increase
				locat_clus=GetFatValue12(locat_clus);///FAT12******************************************
			}while(locat_clus!=FAT12_EOF);///FAT12******************************************
			displayEntryShort(entr_sh[0],*cnt_entr_sh,sav_cnt_entr_old);//di///FAT12******************************************splay new add list
		}	
	}
}
int getDataFile(Entry_Short *entr_sh,int index){				//////////////DATA/////////
	
	printf("\n\nName:");
	print_Str(&entr_sh[index].name[0],ENTRY_FILE_NAME_BYTE);
	print_Str(&entr_sh[index].ext[0],ENTRY_FILE_NAME_EXT_BYTE);
	printf("\n");
	
	int check=((entr_sh[index]).attr==0x10);
	if(check){
		printf("--->folder has been opened!\n");
		return 0;
	}
	int locat_clus=(entr_sh[index]).first_clus_low;
	do{
		int i;
		int offset=((locat_clus-2)+0x21)*0x200;//data location
		//printf("\nofset:%d\n",offset);
		Shift_Offset(offset);
		for(i=0;i<(512*Boot->sec_per_clus);i++){
			printf("%c",fgetc(fp));
		}
		locat_clus=GetFatValue12(locat_clus);///FAT12******************************************
		//printf("\nlocat_clus:%p\n",locat_clus);
	}while(locat_clus!=FAT12_EOF);///FAT12*****************************************************
	return 1;
}

void readDataNode(){											////////////DATA/////////
	callBootSector();			

	initList();
	//create node
		
	int cnt_entr_sh;
	int cnt_entr_sh_sub;
	int offset=(Boot->resv_sec_cnt+Boot->sec_per_FAT*Boot->num_FAT)*0x200;//root directory location
	int cnt_clus_root=Boot->root_entr_cnt*32/512;//number of clusters in root
	
	Entry_Short *entr_sh=readEntryShort(offset,&cnt_entr_sh);//cnt_entr_sh=7
	displayEntryShort(&entr_sh[0],cnt_entr_sh,0);


	printf("////////////*****__*****///////////");
	printf("\nSelect the file to display data by number on the screen:");	
	int temp;
	scanf("%d",&temp);
	temp--;
	int check=(entr_sh[temp].attr!=0x10);
	printf("check:%d\n",check);
	if(check){
		int status=getDataFile(&entr_sh[0],temp);
	}else{
		printf("AAAA\n");
		createNode(&entr_sh[temp]);
		printf("AAAA\n");
		int offset_clus=((head->entr_sh.first_clus_low-2)+0x21)*0x200+32*2;//subfile location
		printf("AAAA\n");
		Entry_Short *entr_sh_sub=readEntrInClusNode(offset_clus,&cnt_entr_sh_sub,head->entr_sh.first_clus_low);
		printf("AAAA\n");
		//checkFileNode(entr_sh[temp].first_clus_low);
		displayEntryShort(&entr_sh_sub[0],cnt_entr_sh_sub,0);
	}	
	
}

void checkFileNode(int locat_clus){								////////////DATA////////
	
}

/////////////////////////DATA -end///////////////////////////////////////////////////

#endif
