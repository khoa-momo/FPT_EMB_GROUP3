#include"FAT_system.h"


int main()
{
	FILE*fptr1=fopen("floppy_2.img","r");
	char arr[100]="doc.txt";
	FILE*fptr2=fopen(arr,"w");	
	Boot_Sector Boot={0};
	
	
//	int i,temp;
//	for(i=0;i<512;i++)
//	{
//		printf("%c",fgetc(fptr1));
//	}

	getBootSector(fptr1,&Boot);
	displayBoot(Boot);
	
	
	fclose(fptr2);
	fclose(fptr1);
	return 0;
}
