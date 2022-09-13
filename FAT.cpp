#include "FAT.h"
///////Read N Byte From Offset

///////////////////////////////////////////////////////////////
void CreatList(List *l)
{
    l->RootDirectory=NULL;
    l->tail=NULL;
}
Node *CreatNode(File *f)
{
    Node *node=(Node *)malloc(sizeof(Node));
    node->file=f;
    node->next=NULL;
    return node;
}
void DeleteFinalNode(List *l)
{
    Node *p=l->RootDirectory;
    while((p->next)->next!=NULL)
    {
        p=p->next;
    }
    p->next=NULL;
}
void AddNode(List *l,Node *p)
{
    if(l->RootDirectory==NULL)
    {
        l->RootDirectory=p;
    }
    else{
        Node *q=l->RootDirectory;
        while(q->next!=NULL)
        {
            q=q->next;
        }
        q->next=p;
    }
}
void DisplayList(List *l)
{   printf("Directory:");
    if(l->RootDirectory==NULL)
    {
        printf("hihi");
    }
    else
    {
        Node *p=l->RootDirectory;  
        while(p->next!=NULL)
        {
        File *f=p->file;
        printf("%s/",f->name);
        p=p->next;  
    }
    }
}
////////////////////////////////////////////////////////////

int ReadnByte(int n,int offset,FILE *fp)
{  
    int *buff=(int *)malloc(n*sizeof(int));
    fseek(fp,offset,SEEK_SET);
    for(int i=0;i<n;i++)
    {
        buff[i]=fgetc(fp);
    }
    int c=buff[0];
    for(int i=0;i<n-1;i++)
        { 
             c|=(buff[i+1]<<(8*(i+1)));
        }
    return c;
}
FatType TypeofFAT(FILE *fp)
{   
    FatType flag=FAT12;
    fseek(fp,0x36,SEEK_SET);
    // uint16_t *buffer=(uint16_t*)malloc(5*sizeof(uint16_t));
    char buffer[5];
    for(int i=0;i<5;i++)
    {
        buffer[i]=fgetc(fp);
    }

    if(buffer[3]=='1'&&buffer[4]=='2')
    {
        flag=FAT12;
    }
    else if(buffer[3]=='1'&&buffer[4]=='6')
    {
        flag=FAT16;
    }
    //offset 0x52
    else
        flag=FAT32;
   return flag;
}
char *Hex2Char(int n,int offset,FILE *fp)
{
    char *s= (char*)malloc(n* sizeof(char) );
    fseek(fp,offset,SEEK_SET);
    for(int i=0;i<8;i++)
    {
        s[i]=fgetc(fp);
    }
    return s;
}
Boot Read_Boot(FILE *fp)
{
    Boot bootsector;
    bootsector.Fat_Type=TypeofFAT(fp);
    switch (bootsector.Fat_Type)
    {
    case 0:
    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
    bootsector.numfat=ReadnByte(1,NumFAT,fp);
    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
    bootsector.numentry=ReadnByte(2,NumEntry,fp);
    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
        break;
    case 1:
    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
    bootsector.numfat=ReadnByte(1,NumFAT,fp);
    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
    bootsector.numentry=ReadnByte(2,NumEntry,fp);
    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
    case 2: 
    bootsector.secperclus=ReadnByte(1,SecPerClus,fp);
    bootsector.numfat=ReadnByte(1,NumFAT,fp);
    bootsector.bytepersec=ReadnByte(2,BytePerSec,fp);
    bootsector.numentry=ReadnByte(2,NumEntry,fp);
    bootsector.secbeforefat=ReadnByte(2,SecBeForeFAT,fp);
    bootsector.secperfat=ReadnByte(2,SecPerFat,fp);
    default:
        break;
    }

    return bootsector;
}
void Displayboot(Boot boot)
{
    printf("Kieu FAt:%d\n",boot.Fat_Type);
    printf("So Sector truoc Fat:%d\n",boot.secbeforefat);
    printf("So Sector moi Cluster:%d\n",boot.secperclus);
    printf("So Entry trong Root:%d\n",boot.numentry);
    printf("So luong Fat:%d\n",boot.numfat);
    printf("So luong Sec moi FAT:%d\n",boot.secperfat);
}
File* Read_File(FILE *fp)
{
    Boot boot =Read_Boot(fp);
    File *file=(File*)malloc(224*sizeof(File));
    //find the Root Directory
    // Sec cua Root= So luong Fat * Kich thuoc Fat+ BootSector
    int RootSector=boot.numfat*boot.secperfat+boot.secbeforefat;
    //find subfolder and file
    // file
    int offset =0x20;
    int pointer=RootSector*0x200;
    int c=0;
    for(int pointer=RootSector*0x200;pointer<0x4200;pointer+=0x20)
        { 
            while(ReadnByte(1,pointer+0x0b,fp)==0x0f)
            {
                pointer+=0x20;
            }
                file[c].name=Hex2Char(8,pointer+Filename,fp);
                file[c].extension=Hex2Char(3,pointer+NameExten,fp);
                file[c].FirstClus=ReadnByte(2,pointer+FisrtClus,fp);
                ++c;     
        }
    return file;
}
int count_Subfolder(int pointer,FILE *fp)
{
        int c=0;
        while(ReadnByte(1,pointer,fp)!=0)
            {  
                // bo qua entry
                while(ReadnByte(1,pointer+0x0b,fp)==0x0f)
                    {
                    pointer+=0x20;
                    }  
                    pointer+=0x20;
                    c++;
            }
        return c;
}
File *ScanRoot(FILE *fp)
{
    Boot boot =Read_Boot(fp);
    int offset =0x20;
    int RootSector=boot.numfat*boot.secperfat+boot.secbeforefat;
    int pointer=RootSector*0x200;
    int c=0;
    //dem so file con 
    int m=count_Subfolder(0x2600,fp);
    File *f=(File*)malloc(m*sizeof(File));
    while(ReadnByte(1,pointer,fp)!=0)
        {  
            // bo qua entryphu
             while(ReadnByte(1,pointer+0x0b,fp)==0x0f)
                {
                    pointer+=0x20;
                }  
                f[c].name=Hex2Char(8,pointer+Filename,fp);
                f[c].extension=Hex2Char(3,pointer+NameExten,fp);
                f[c].FirstClus=ReadnByte(2,pointer+FisrtClus,fp);  
                 printf("%d:%s\n",c,f[c].name);
                pointer+=0x20;
                c++;
        }
    return f;
}
File *ScanFolder(File *f,FILE *fp)
{
    int offset =0x20;
    int pointer=(f->FirstClus+0x1f)*0x200+0x40;
    int c=0;
    //dem so file con 
    int m=count_Subfolder(pointer,fp);
    File *subf=(File*)malloc(m*sizeof(File));
    while(ReadnByte(1,pointer,fp)!=0)
        {  
            // bo qua entryphu 
                subf[c].name=Hex2Char(8,pointer+Filename,fp);
                subf[c].extension=Hex2Char(3,pointer+NameExten,fp);
                subf[c].FirstClus=ReadnByte(2,pointer+FisrtClus,fp);  
                printf("%d:%s\n",c,subf[c].name);
                pointer+=0x20;
                c++;
        }
    return subf;
}
void Go2Folder_File(FILE *fp,List *l,File *f)
{
    ///Them Node chua File vao List
    
    int index;
    printf("Nhap so thu tu file can doc:");
    scanf("%d",&index);
    Node *node=CreatNode(&f[index]);
    AddNode(l,node);
    /// Folder///////////////// /////
    if(f[index].extension[0]==' ')
        {
            Go2Folder_File(fp,l,ScanFolder(&f[index],fp));
        }
    else{
        /// hien thi data
    }
    
}
int main()
{   
    FILE *fp=fopen("floppy.img","r");
    //Scan_Folder(&file[5],fp,l);
    //Scan_Folder();
    List *l;
    printf("1");
    CreatList(l);
    printf("2");
    File *f=ScanRoot(fp);
    Go2Folder_File(fp,l,f);
    return 0;
}
