#include"HAL.c"

int main(){
	fp = fopen("floppy.img","rb");
	  
	//readData(); 
 	readDataNode();
	fclose(fp);
	
	return 0;
}
