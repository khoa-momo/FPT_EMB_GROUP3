#include "READFAT.h"


int main(){
//    FILE *fptr = fopen("C:\\Users\\H\\Desktop\\FPT\\mock 1\\floppy.img","r");
    FILE *fptr = fopen("floppy.img","r");
    if(fptr == NULL){
        printf("NO FIND FILE");
    }
    else{
        printf("%d" , GetFatValue12(fptr , 9));
    }
}
