
#include"FAT_system.cpp"

int main(){
	FILE *fptr = fopen("floppy.img","r");
	
	int whatever;	
	Shift_Offset(9,0);
	for(int i=0; i<1; i++){
		whatever+=(fgetc(fptr)<<(8*i));
	}
	printf("||whatever: %x||\n",whatever);
	
	//1.
	Boot_Sector Boot = {0};
	getBootSector(fptr, &Boot);
	displayBoot(Boot);
	
	//Find Root Directory
	int RootDirectoryStart = findRootDirectory(Boot);//2600
	printf("\nrootDirect: %x\n",RootDirectoryStart);
	
//	struct Entr_Main_Root[5];
//	for(int i=0; i<5; i++){
//	}
	
	
	//2.
	Entr_Main_Root Root = {0};
	getRootSector(fptr, &Root, RootDirectoryStart);
	displayRoot(Root);
	
	//3.
	findClusStart(Boot,Root);
	
	printf("\n\n\n"); 
    printf("~~~~~~~~~~~~~~~~~ROOT ATTRIBUTE~~~~~~~~~~~~~~~~~\n");
	Entr_Main_Root *array = foo2(fptr, &Boot, 6); 
	for(int i=0; i<6; i++){
    	printf("############%d############\n",i);
    	printf("name: %s\n", (array+i)->name);
		printf("First_cluster: %d\n", (array+i)->first_clus);
		printf("Size: %d\n\n", (array+i)->size);
	}
	
	
	
	
	
	
	
	
	
	
	
	fclose(fptr);


	return 0;
}
