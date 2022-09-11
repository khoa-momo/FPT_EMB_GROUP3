#include"FAT_system.h"
#include"FAT_system.c"

int main(){
	fp=fopen("floppy.img","r");
	char arr[100]="doc.txt";
	FILE*fp2=fopen(arr,"w");	
	 
	int i;
	//*print root block
	Shift_Offset(0x2600);
	for(i=0;i<512;i++){
		printf("%c ",fgetc(fp));
	}

	//1.Boot_Sector
	Boot_Sector *Boot = readBootSector();
	displayBoot(*Boot);
	
	//2.
	readData();
  
  
	fclose(fp2); 
	return 0;
}
