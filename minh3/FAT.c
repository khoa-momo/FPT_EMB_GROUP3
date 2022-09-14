#include "FAT.h"
//Read N Byte From Offset


/*----------------------LINKED LIST----------------------*/
void CreatList(List *l){
    l->RootDirectory=NULL;
    l->tail=NULL;
}

Node *CreatNode(File *f){
    Node *node=(Node *)malloc(sizeof(Node));
    node->file=f;
    node->next=NULL;
    return node;
}

void DeleteFinalNode(List *l){
    Node *p=l->RootDirectory;
    while((p->next)->next!=NULL){
        p=p->next;
    }
    p->next=NULL;
}

void AddNode(List *l,Node *p){
    if(l->RootDirectory==NULL){
        l->RootDirectory=p;
    }
    else{
        Node *q=l->RootDirectory;
        while(q->next!=NULL){
            q=q->next;
        }
        q->next=p;
    }
}

void DisplayList(List *l){   
	printf("Directory:");
    if(l->RootDirectory==NULL){
        printf("hihi");
    }else{
        Node *p=l->RootDirectory;  
        while(p->next!=NULL){
	        File *f=p->file;
	        printf("%s/",f->name);
	        p=p->next;  
	    }
    }
}
/*---------------------------------------------------------*/

/*READ BYTE*/
int ReadnByte(int n,int offset,FILE *fp){  
    int *buff=(int *)malloc(n*sizeof(int));
    fseek(fp,offset,SEEK_SET);
    for(int i=0;i<n;i++){
        buff[i]=fgetc(fp);
    }
    int c=buff[0];
    for(int i=0;i<n-1;i++){ 
        c|=(buff[i+1]<<(8*(i+1)));
    }
    return c;
}

/*READ CHAR(array)*/
char *Hex2Char(int n, int offset, FILE *fp){
    char *s = (char*)malloc(n* sizeof(char));
    fseek(fp,offset,SEEK_SET);
    for(int i=0;i<8;i++){
        s[i] = fgetc(fp);
    }
    return s;
}

FatType TypeofFAT(FILE *fp){   
    FatType flag = FAT12;
    fseek(fp,0x36,SEEK_SET);
    
    // uint16_t *buffer=(uint16_t*)malloc(5*sizeof(uint16_t));
    char buffer[5];
    for(int i=0;i<5;i++){
        buffer[i]=fgetc(fp);
    }

    if(buffer[3]=='1'&&buffer[4]=='2'){
        flag=FAT12;
    }
    else if(buffer[3]=='1'&&buffer[4]=='6'){
        flag=FAT16;
    }
    //offset 0x52
    else
        flag=FAT32;
    return flag;
}

/*---------------------------BOOT-----------------------------*/
Boot Read_Boot(FILE *fp){
    Boot bootsector;
    bootsector.Fat_Type=TypeofFAT(fp);
    switch (bootsector.Fat_Type){
	    case 0:
		    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
		    bootsector.numfat=ReadnByte(1,NumFAT,fp);
		    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
		    bootsector.numentry=ReadnByte(2,NumEntry,fp);
		    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
		    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
	        break;
	    case 1:
		    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
		    bootsector.numfat=ReadnByte(1,NumFAT,fp);
		    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
		    bootsector.numentry=ReadnByte(2,NumEntry,fp);
		    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
		    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
		    break;
	    case 2: 
		    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
		    bootsector.numfat=ReadnByte(1,NumFAT,fp);
		    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
		    bootsector.numentry=ReadnByte(2,NumEntry,fp);
		    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
		    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
		    break;
	    default:
	        break;
	}
    return bootsector;
}

void Displayboot(Boot boot){
    printf("Kieu FAt:%d\n",boot.Fat_Type);
    printf("So Sector truoc Fat:%d\n",boot.secbeforefat);
    printf("So Sector moi Cluster:%d\n",boot.secperclus);
    printf("So Entry trong Root:%d\n",boot.numentry);
    printf("So luong Fat:%d\n",boot.numfat);
    printf("So luong Sec moi FAT:%d\n",boot.secperfat);
}
/*---------------------------------------------------------*/

//File* Read_File(FILE *fp){
//	printf("Read_File\n");
//    Boot boot =Read_Boot(fp);
//    File *file=(File*)malloc(224*sizeof(File));
//    // find the Root Directory
//    // Sec cua Root = So luong Fat * Kich thuoc Fat+ BootSector
//    int RootSector=boot.numfat*boot.secperfat+boot.secbeforefat;
//    // find subfolder and file
//    // file
//    int offset = 0x20;
//    int pointer = RootSector*0x200;
//    int c = 0;
//    for(int pointer = RootSector*0x200; pointer<0x4200; pointer+=0x20){ 
//        while(ReadnByte(1,pointer+0x0b,fp)==0x0f){
//            pointer+=0x20;
//        }
//        file[c].name=Hex2Char(8,pointer+Filename,fp);
//        file[c].extension=Hex2Char(3,pointer+NameExten,fp);
//        file[c].FirstClus=ReadnByte(2,pointer+FisrtClus,fp);
//        ++c;     
//    }
//    return file;
//}


/*---------------------------ROOT-----------------------------*/
File *ScanRoot(FILE *fp){
	printf("ScanRoot\n");
	//1. BOOT
    Boot boot = Read_Boot(fp);
    
    //Find Root Sector
    int RootSector = boot.numfat*boot.secperfat+boot.secbeforefat;
    //Root pointer
    int pointer = RootSector*0x200;
    printf("\n\nPointer: %x\n\n",pointer);
    
    //1.Count Entry -> entry_cnt
    int entry_cnt = count_Entry(pointer,fp);
    //Make an array struct of Entry
    File *f = (File*)malloc(entry_cnt*sizeof(File));
    
    int c = 0;
    //Read Root
    while(ReadnByte(1,pointer,fp)!=0){  
		while(ReadnByte(1,pointer+0x0b,fp) == 0x0f){//Pass Sub-Entry
		    pointer += 0x20;
		}  
		f[c].name = Hex2Char(8,pointer+Filename,fp);
		f[c].extension = Hex2Char(3,pointer+NameExten,fp);
		f[c].FirstClus = ReadnByte(2,pointer+FisrtClus,fp);  
		
        f[c].timeCreate = ReadnByte(2,pointer+TimeCreate,fp);  
        f[c].dateCreate = ReadnByte(2,pointer+DateCreate,fp); 
        
        f[c].timeUpdate = ReadnByte(2,pointer+TimeUpdate,fp);  
        f[c].dateUpdate = ReadnByte(2,pointer+DateUpdate,fp);  
         
		printf("\n%d:%s - ",c,f[c].name);
		
//		printf("Time Create: "); readTime(f[c].timeCreate);
//      printf("Date Create: "); readDate(f[c].dateCreate);
        printf("\n\t\t");
        printf("Time Update: "); readTime(f[c].timeUpdate);
        printf("Date Update: "); readDate(f[c].dateUpdate);
        printf("\n");
		
		pointer+=0x20;
		c++;
    }
    
    /*Array struct of Entry*/
    return f;
}

/*Count Sub-Entry and Folder Root*/
int count_Entry(int pointer,FILE *fp){
	printf("count_Subfolder\n");
    int c = 0;
    while(ReadnByte(1,pointer,fp)!=0){  
        //Check attribute 0x0b -> if equal 0x0f -> Sub-Entry -> pass
        while(ReadnByte(1,pointer+0x0b,fp)==0x0f){
        	pointer+=0x20;
        }  
        pointer+=0x20;
        c++;
    }
    return c;
}

File *Scan_Sub_Folder(File *f,FILE *fp){
	printf("ScanFolder\n");
    int offset = 0x20;
    int pointer = (f->FirstClus+0x1f)*0x200+0x40;
    int c = 0;
    
    //Count Entry of the SUB-FOLDER
    int m = count_Entry(pointer,fp);
    File *subf = (File*)malloc(m*sizeof(File));
    while(ReadnByte(1,pointer,fp)!=0){  
    
	// bo qua entryphu 
        subf[c].name = Hex2Char(8,pointer+Filename,fp);
        subf[c].extension = Hex2Char(3,pointer+NameExten,fp);
        subf[c].FirstClus = ReadnByte(2,pointer+FisrtClus,fp);  
        
        subf[c].timeCreate = ReadnByte(2,pointer+TimeCreate,fp);  
        subf[c].dateCreate = ReadnByte(2,pointer+DateCreate,fp); 
        
        subf[c].timeUpdate = ReadnByte(2,pointer+TimeUpdate,fp);  
        subf[c].dateUpdate = ReadnByte(2,pointer+DateUpdate,fp);  
        
        printf("\n%d:%s - ",c,subf[c].name);
//		printf("Time Create: "); readTime(subf[c].timeCreate);
//        printf("Date Create: "); readDate(subf[c].dateCreate);
        printf("\n\t\t");
        printf("Time Update: "); readTime(subf[c].timeUpdate);
        printf("Date Update: "); readDate(subf[c].dateUpdate);
        printf("\n");
        pointer+=0x20;
        c++;
    }
    return subf;
}



void Go2Folder_File(FILE *fp,List *l,File *f){
	printf("Go2Folder_File\n");
    ///Them Node chua File vao List
    int index;
    printf("Nhap so thu tu file can doc:");
    scanf("%d", &index);
    Node *node = CreatNode(&f[index]);
    AddNode(l,node);
    /// Folder///////////////// /////
    if(f[index].extension[0]==' '){
        Go2Folder_File(fp, l, Scan_Sub_Folder(&f[index],fp));
    }
    else{
        /// hien thi data
    }
    
}

/*Time Date Reading*/
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

