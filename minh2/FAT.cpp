#include "FAT.h"
///////Read N Byte From Offset
int numForder = 0;

FatType TypeofFAT(FILE *fp){   
    FatType flag=FAT12;
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

int ReadnByte(int n,int offset,FILE *fp){//n: number of byte
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

char *Hex2Char(int n,int offset,FILE *fp){
    char *s= (char*)malloc(n* sizeof(char));
    fseek(fp,offset,SEEK_SET);
    for(int i=0;i<8;i++){
        s[i]=fgetc(fp);
    }
    return s;
}

//////////////////////////BOOT//////////////////////////
Boot Read_Boot(FILE *fp){
    Boot bootsector;
    bootsector.Fat_Type = TypeofFAT(fp);
    switch(bootsector.Fat_Type){
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
	    case 2: 
		    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
		    bootsector.numfat=ReadnByte(1,NumFAT,fp);
		    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
		    bootsector.numentry=ReadnByte(2,NumEntry,fp);
		    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
		    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
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
 
//////////////////////////ROOT-File//////////////////////////
File *Read_File(FILE *fp){
    Boot boot = Read_Boot(fp);
    File *file = (File*)malloc(224*sizeof(File));
    //find the Root Directory
    // Sec cua Root = So luong Fat * Kich thuoc Fat + BootSector
    int RootSector=boot.numfat*boot.secperfat+boot.secbeforefat;
    //find subfolder and file
    //file
    int offset = 0x20;
    int pointer = RootSector*0x200;
    int c=0;
    for(int pointer = RootSector*0x200;pointer<0x4200;pointer+=0x20){ 
        while(ReadnByte(1,pointer+0x0b,fp)==0x0f){
            pointer += 0x20;
        }
        file[c].name = Hex2Char(8,pointer+Filename,fp);
        file[c].extension = Hex2Char(3,pointer+NameExten,fp);
        file[c].FirstClus = ReadnByte(2,pointer+FisrtClus,fp);
        ++c;     
    }
    return file;
}

File file[244];
static int count_filefolder = -1;

//Scan Root
void Scan_Folder(int pointer, FILE *fp){//0x2600   
    while(ReadnByte(1,pointer,fp) != 0){//Read till reach 1st 0  
        count_filefolder++;
        
        //skip Sub Entry - 0x0f
        while(ReadnByte(1,pointer+0x0b,fp)==0x0f){
            pointer+=0x20;//skip 32 byte
        }
        
        //TH1: Main Entry - File
        file[count_filefolder].name = Hex2Char(8,pointer,fp);
        file[count_filefolder].extension = Hex2Char(3,pointer+NameExten,fp);
        file[count_filefolder].FirstClus = ReadnByte(2,pointer+FisrtClus,fp);
        
        //TH2: Folder -> Re-route to Sub-Folder Entries || 1st Clus of Sub Folder
//        if(file[count_filefolder].extension[1] == ' '){
//        	//Scan_Folder(pointer, fp)
////        	printf("||||%x||||\n", (file[count_filefolder].FirstClus+0x1f)*0x200 +0x40);
//            Scan_Folder((file[count_filefolder].FirstClus+0x1f)*0x200 +0x40, fp);
//        } 
        pointer+=0x20; 
    }
}
  
int main(){   
    FILE *fp=fopen("floppy.img","r");
    // File *file=Read_File(fp);
    // printf("%s",file[223].name);
    char ext[]="folder";
    
    Scan_Folder(0x2600,fp);
//    printf("count_filefolder: %d\n\n", count_filefolder);
//    for(int i=0; i<count_filefolder; i++){
//        printf("name: %s \t\t FirstClus: %d\n", file[i].name, file[i].FirstClus);
//    }
    printf("\n\n\n");
    
    int choose;
    while(true){
    	int folder_index = 1;
	    for(int i=0; i<count_filefolder; i++){ 
	    	if(file[i].extension[1] != ' '){
	    		printf("%d) File name: %s \t\t FirstClus: %d\n",i+1, file[i].name, file[i].FirstClus);
			}else{
				printf("%d) Folder name: %s \t\t FirstClus: %d\n",folder_index, file[i].name, file[i].FirstClus);
				folder_index++;
			} 
	    } 
		printf("\nChoose: "); scanf("%d",choose); 
		Scan_Folder((file[count_filefolder].FirstClus+0x1f)*0x200 +0x40, fp);
	}
    
    
    return 0;
}
