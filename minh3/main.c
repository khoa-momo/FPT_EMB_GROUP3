#include "FAT.h"
#include "FAT.c"

int main(){   
    FILE *fp = fopen("floppy.img","rb");
    //Scan_Folder(&file[5],fp,l);
    //Scan_Folder();
     
    //List here is a struct for LL 
    List *l; //a node of 'List' - 1st Node?
    CreatList(l);
     
    File *f = ScanRoot(fp);
     
    Go2Folder_File(fp,l,f);
    return 0;
}
