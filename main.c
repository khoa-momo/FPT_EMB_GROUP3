#include "READFAT.h"
/* declare entry main */
typedef struct
{
    uint16_t first_cluster;
    uint16_t Name[11];

} ENTRY_MAIN;
ENTRY_MAIN EntryMain;

/* declare node entry main */
struct NODE_OF_ENTRY
{
    ENTRY_MAIN DATA;
    struct NODE *pNext;

};
typedef struct NODE_OF_ENTRY NodeOfEntry;

int main()

{
    FILE *fptr = fopen("C:\\Users\\H\\Desktop\\FPT\\mock 1\\floppy.img","r");
    if(fptr == NULL)
    {
        printf("NO FIND FILE");
    }
    else
    {
        GetBoostector(fptr);
        ReadOneEntry(fptr);
        PrinTestOneEntry(EntryMain);

        NodeOfEntry *pHead = NULL;

        ReadAllEntryMain(fptr, &pHead);

        printf("\n Created Linked list is: ");

    }
}
