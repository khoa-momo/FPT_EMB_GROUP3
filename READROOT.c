#include"READFAT.h"

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

/* add first node */

void AddEntryToNode(NodeOfEntry *(*pHead), ENTRY_MAIN Data)
{
    NodeOfEntry *temp = pHead;
    NodeOfEntry *New_Node = (NodeOfEntry*)malloc (sizeof(NodeOfEntry));
    New_Node->DATA = Data;
    New_Node->pNext = NULL;
    if(*pHead == NULL)
    {
        *pHead = New_Node;
    }
    else
    {
        while (temp->pNext != NULL)
        {
            temp = temp->pNext;
        }
        temp->pNext = New_Node;
    }
}


void ReadOneEntry (FILE *fptr)
{
    char temp;

        SET_FILE_POITER(FIRST_ADDRESS_OF_ROOT);

        for(int i = 0; i <11 ; i++)
        {
            EntryMain.Name[i] = fgetc(fptr);
        }

        SET_FILE_POITER(FIRST_ADDRESS_OF_ROOT + 0x1A);

        for(int i = 0; i < 2; i++)
        {
            EntryMain.first_cluster += (fgetc(fptr) << (8 * i));
        }

}
 void PrinTestOneEntry (ENTRY_MAIN test)
 {
     for(int i = 0; i<11; i++)
     {
         printf("%c", test.Name[i]);
     }

     printf("\n");

     printf("%d\n", test.first_cluster);
 }

void ReadAllEntryMain (FILE *fptr, NodeOfEntry *(*pHead))
{
    char temp;
    SET_FILE_POITER(FIRST_ADDRESS_OF_ROOT);

    for(int i =0; i < 32 * BootSector.count_entry_of_rdet; i +=32)
    {
        SET_FILE_POITER(FIRST_ADDRESS_OF_ROOT + i);
        if(temp = fgetc(fptr) != NULL)
        {
            SET_FILE_POITER(FIRST_ADDRESS_OF_ROOT + i + 0x0B);
            if(temp = fgetc(fptr) != 0x0F)
            {
                ReadOneEntry(fptr);
                AddEntryToNode(&pHead, EntryMain);
            }
        }
        else
        {
            break;
        }
    }
}

void printList(NodeOfEntry *node)
{
  while (node != NULL)
  {
     printf(" %d ", node->DATA);
     node = node->pNext;
  }
}



