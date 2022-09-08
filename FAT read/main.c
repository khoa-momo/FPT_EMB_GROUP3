#include"FAT_system.h"
#include"FAT_system.c"

int main(){
	FILE*fptr1=fopen("floppy.img","r");
	
	//1.
	Boot_Sector Boot = {0};
	getBootSector(fptr1, &Boot);
	displayBoot(Boot);
	
	//Find Root Directory
	int RootDirectoryStart = findRootDirectory(Boot);
	printf("\nrootDirect: %x\n",RootDirectoryStart);
	
	
	//2.
	Entr_Main_Root Root = {0};
	getRootSector(fptr1, &Root, RootDirectoryStart);
	displayRoot(Root);
	
	//3.
	findClusStart(Boot,Root);
	
	
	
	fclose(fptr1);
	char arr[100]="doc.txt";
	FILE*fptr2=fopen(arr,"w");	
	fclose(fptr2);
	return 0;
}