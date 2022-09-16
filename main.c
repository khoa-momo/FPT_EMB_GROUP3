#include"HAL.h"


int main(int argc, char *argv[]){
	fp = fopen(argv[1],"rb");
 	callData();
	fclose(fp);
	return 0;
}
