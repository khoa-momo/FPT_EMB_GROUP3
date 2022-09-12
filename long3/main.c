#include"FAT_system.h"
//#include"FAT_system.c"

int main()
{
	fp=fopen("floppy_2.img","r");
	char arr[100]="doc.txt";
	FILE*fp2=fopen(arr,"w");	
	
	
	
	int i;
	Shift_Offset(0x2600);
	for(i=0;i<512;i++)
	{
		printf("%c",fgetc(fp));
	}

	Boot_Sector *Boot=readBootSector();
	displayBoot(*Boot);
	
	readData();
 
	
	fclose(fp2);
	//fclose(fp);
	return 0;
}
