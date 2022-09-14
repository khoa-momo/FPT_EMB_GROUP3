#include"FAT_system.h"
#include"FAT_system.c"

int main()
{
	fp=fopen("floppy_2.img","rb");
	
//	int i;
//	Shift_Offset(0x300);
//	for(i=0;i<512;i++){
//		printf("%x-",fgetc(fp));
//	}

	//readData();
 	readDataNode();
	//fclose(fp);
	return 0;
}
