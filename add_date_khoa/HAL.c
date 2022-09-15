#ifndef	__HAL_C__
#define	__HAL_C__
#include"FAT_system.c"

/*--------------------------- I/ LINK LIST - START ---------------------------*/
void initList(){						//create list							/////linklist///////
	head = NULL;
}

bool isEmpty(){							//check list empty						/////linklist//////
	if(head==NULL){
		return true;
	}
	return false;
}

void createNode(Entry_Short *entr_sh){	//create node with data					////linklist//////
	printf("\n### createNode ###\n");		
	Node *node = (Node*)calloc(1,sizeof(Node));
	//1.Add in an empty node
	addNodeToList(node);
	int i;
	//2.Fill attr: name & first_clus_low to that node
	for(i=0;i<ENTRY_FILE_NAME_BYTE;i++){
		(node->entr_sh).name[i]=entr_sh[0].name[i];
	}
	(node->entr_sh).first_clus_low=entr_sh[0].first_clus_low;
	printLinkedList(head);
}

//add First Node to list
void addNodeToList(Node *node){													////linklist/////
	printf("\n### addNodeToList ###\n");		
	if(isEmpty()){
		printf("empty\n");
		head=node;
		head->next=NULL;
	}
	else{
		printf("not empty\n");
		node->next=head;
		head=node;
	}
}

void removeNode(){						//delete node
	printf("\n### removeNode ###\n");							////linklist////
	Node *last_Node = head;
	//List empty
	if(isEmpty()){
		printf("empty\n");
	}
	//List has 1 Node only
	else if(head->next==NULL){
		free(last_Node);
		head=NULL;		
	}
	//List has >1 Node only
	else{	
		head=head->next;	
	}
	printLinkedList(head);
}
/*--------------------------- LINK LIST - END --------------------------*/


/*--------------------------- HAL - START ---------------------------*/
void callBootSector(){											//DATA//
	Boot = readBootSector();
	displayBoot(*Boot);
}

void checkFile(Entry_Short *entr_sh, int cnt_entr_sh){			//DATA//
	printf("\n### CheckFile ###\n");
	int check;
	int temp;
	char input;
	int offset_clus;
	
	Entry_Short *entr_sh_sub = entr_sh;
	int cnt_entr_sh_sub = cnt_entr_sh;
	int cnt_entr_cpy = cnt_entr_sh_sub;
	
	int status = 0;
	while(1){										//open while
		printf("\n//////////////////////////////**********----**********//////////////////////////////\n");
		
		//status==1 --- open
		if(status==1){			
			printf("\n[[[status==1]]]\n");				
			printf("\nSelect the function by entering dir number:\n");
			printf("\tx. Out program\n");
			printf("\tb. Back to previous folder\n");
			
			do{
				printf("Enter your selection:");
				scanf("%c", &input);
				fflush(stdin);
			}while(input!='x' && input!='b');
			
			//temp=='x': loop out of program
			if(input == 'x'){
				return;
			}
			//temp=='b': go back						
			else if(input == 'b'){
				status = 0;														//resert status
				displayEntryShort(&entr_sh_sub[0], cnt_entr_cpy, 0);			//display folder before
			} 
		}//status==1 --- close
		 
		//status==0 --- open
		else{		
			/*I/ Status 0, Get input*/			
			printf("\n[[[status==0]]]\n");				
			printf("\nSelect the function or open the file by entering dir number:\n");	
			printf("\tx. Out program\n");
			//Back file if not empty list
			(isEmpty()!=1)?printf("\tb. Back to previous folder\n"):printf("");	
			
			do{
				printf("Enter your selection: ");
				scanf("%c",&input);
				fflush(stdin);
				((isEmpty()==1)&&(input==98))?(input=47):printf("");			//You can not input 'b' if not empty list
			}while((input<48||((input>(cnt_entr_sh_sub+48))&&(input!='x'&&input!='b'))));
			//ascii: 	b-98		x-120	number + 48
			
			/*II/ Processing input*/
			//TH1: input=='x' 
			if(input=='x'){				
				return;
			}
					
			//TH2: input=='b' 					
			else if(input=='b'){		
				//2.1 - remove node first		
				removeNode();
				//2.2 - if list isEmpty callback to entryRoot 
				if(isEmpty()){	
					//Show root entry
					printf("\n(((Back to root)))\n");
					
					cnt_entr_sh_sub=cnt_entr_sh;								
					
					entr_sh_sub=entr_sh;
					displayEntryShort(&entr_sh_sub[0],cnt_entr_sh_sub,0);
					cnt_entr_cpy=cnt_entr_sh_sub;
				}
				//2.3 - else callback to node first in list
				else{															
					printf("\n(((Back to bef node)))\n");		
					offset_clus=((head->entr_sh.first_clus_low-2)+bef_data_sector)*Boot->byte_per_sec+32*2;
					
					entr_sh_sub=readEntrInClus(offset_clus,&cnt_entr_sh_sub,head->entr_sh.first_clus_low);		
					displayEntryShort(&entr_sh_sub[0],cnt_entr_sh_sub,0);
					cnt_entr_cpy=cnt_entr_sh_sub;//copy count entry scaned
				}
			}	
			
			//TH3: input is number -> Go to Sub-Folder or Print Data						
			else{//input defalse --- open
				temp=(int)input-48;						//swap char to int
				check=(entr_sh_sub[temp].attr!=0x10);	//check if file
				//check==1 -> File
				if(check){				
					//Read Data	
					status = getDataFile(&entr_sh_sub[0], temp);
				}
				//check false -> Folder
				else{							
					//int cnt_entr_sh_sub=0;
					//create node in first list -> Go to Sub-Folder
					createNode(&entr_sh_sub[temp]);	
					//Get first_clus_low of folder (0x1a)
					
					offset_clus=((head->entr_sh.first_clus_low-2)+bef_data_sector)*Boot->byte_per_sec+32*2;
					entr_sh_sub=readEntrInClus(offset_clus,&cnt_entr_sh_sub,head->entr_sh.first_clus_low);		
					displayEntryShort(&entr_sh_sub[0],cnt_entr_sh_sub,0);
					cnt_entr_cpy=cnt_entr_sh_sub; //copy count entry scaned
				}//check defalse --- close					
			}//input defalse --- close	
			
		}//status==0 	--- close	
	}//close while
}

void readDataNode(){	
	//1. BOOT		
	printf("--------- BOOT ---------\n");								//DATA//
	callBootSector();		
	printf("------------------------\n");	
	
	//2. Init Link List
	initList();
	
	//3. Find Root offset and Root's Sectors
	int cnt_entr_sh;
	
	/*root directory location: 0x2600*/
	//Root_offset = offset_Root(Boot->num_FAT, Boot->sec_per_FAT, Boot->resv_sec_cnt);
	
	main_offset();
	//offset(Boot->num_FAT, Boot->sec_per_FAT, Boot->resv_sec_cnt);
	printf("#root offset: %x\n",Root_offset); 
	
	
	/*Number of clusters/sectors/block in root*/
//	int cnt_clus_root = Boot->root_entr_cnt*32/512;
//	printf("cnt_clus_root: %d",cnt_clus_root); 
	
	//4. Print Root main entries
	Entry_Short *entr_sh = readEntryShort(Root_offset, &cnt_entr_sh);//cnt_entr_sh=7
	displayEntryShort(&entr_sh[0], cnt_entr_sh, 0); 
	
	//5. Go to Sub-Folders
	checkFile(&entr_sh[0], cnt_entr_sh);	
}

/*--------------------------- HAL - END ---------------------------*/

#endif
