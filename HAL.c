#include "HAL.h"

// void DisplayFile(FILE *fp)
// {
//     ScanRoot(fp);
// }
void HAL_Access2File(FILE *fp,int index,List *l)
{
    File *file=ScanRoot(fp);
    Go2Folder_File(fp,l,file,index);
}
// void main()
// {
//     File *fp=fopen("floppy.img","r");
//     List *l;
//     CreatList(l);
//     char c;
//     int index;
//     do{
        
//         scanf("%d",index);
//         HAL_Access2File(fp,index,l);
//     }
//     while(1);

// }
// void main()
// {   //lay ra cac file
//     FILE *fp=fopen("floppy.img","r");
//     // //Scan_Folder(0x2600,fp);
//     // List *l;
//     // CreatList(l);
//     // //Node *p=CreatNode(file[0]);
//     // HAL_DisplayAllFile(fp);
//     DisplayFile(fp);
// }