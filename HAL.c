#ifndef ___HAL_C___
#define ___HAL_C___

#include"HAL.h"

void callBootSector(){											
	Boot = readBootSector();
	displayBoot(*Boot);
}

FAT checkFAT(){
	FAT fat;
	if((Boot->FAT_type[3]=='1')&&(Boot->FAT_type[4]=='2')){
		fat=FAT12;
	}else if((Boot->FAT_type[3]=='1')&&(Boot->FAT_type[4]=='6')){
		fat= FAT16;
	}else{
		fat=FAT32;
	}
	return fat;
}

uint16_t GetFatValue( uint16_t PrsClus){
	uint16_t addrFAT;
	if(checkFAT()==FAT12){
		addrFAT= GetFatValue12(PrsClus);
	}else if(checkFAT()==FAT16){
		addrFAT= GetFatValue16(PrsClus);
	}
	return addrFAT;
}


/*--------------------------- V/ DATA - START -------------------------*/
int getDataFile(Entry_Short *entr_sh, int index){				/*DATA*/
	printf("\n\n### GetDataFile ###\n");
	printf("\nFile-opening:");
	print_Str(&entr_sh[index].name[0],ENTRY_FILE_NAME_BYTE);
	print_Str(&entr_sh[index].ext[0],ENTRY_FILE_NAME_EXT_BYTE);
	printf("\n");
	
	int locat_clus = (entr_sh[index]).first_clus_low;
	do{
		int i;
		int offset = ((locat_clus-2)+bef_data_sector)*Boot->byte_per_sec;//data location
		Shift_Offset(offset);
		prtByteFile(Boot->byte_per_sec*Boot->sec_per_clus);
		locat_clus = GetFatValue(locat_clus);///FAT12 
	}while(locat_clus!=FAT12_EOF);///FAT12 
	return 1;
}
/*-------------------------------------------------------------------*/


/*--------------------------- VI/ MAIN - START -----------------------*/
void checkFile(Entry_Short *entr_sh, int cnt_entr_sh){			/*DATA*/
	int check;
	int temp;
	char input;
	int offset_clus;
	
	Entry_Short *entr_sh_sub = entr_sh;
	int cnt_entr_sh_sub = cnt_entr_sh;
	int cnt_entr_cpy = cnt_entr_sh_sub;
	
	int status = 0;
	while(1){										//open while
		printf("\n\n////////////////////////////////////////=======*****=======\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
		//status==1 --- open
		if(status==1){							
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
			printf("\nSelect the function or open the file by entering dir number:\n");	
			printf("\tx. Out program\n");
			//Back file if not empty list
			(isEmpty()!=1)?printf("\tb. Back to previous folder\n"):printf("");	
			
			do{
				printf("Enter your selection: ");
				scanf("%c",&input);
				fflush(stdin);
				((isEmpty()==1)&&(input==98))?(input=47):printf("");			//You can not input 'b' if not empty list
			}while((input<48||((input>(cnt_entr_cpy+47))&&(input!='x'&&input!='b'))));
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
					
					cnt_entr_sh_sub=cnt_entr_sh;								
					
					entr_sh_sub=entr_sh;
					displayEntryShort(&entr_sh_sub[0],cnt_entr_sh_sub,0);
					cnt_entr_cpy=cnt_entr_sh_sub;
				}
				//2.3 - else callback to node first in list
				else{																
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

void readDataNode(){											/*DATA*/
	//3. Find Root offset and Root's Sectors
	int cnt_entr_sh;
	/*root directory location: 0x2600*/
	//Root_offset = offset_Root(Boot->num_FAT, Boot->sec_per_FAT, Boot->resv_sec_cnt);
	
	//4. Print Root main entries
	Entry_Short *entr_sh = readEntryShort(Root_offset, &cnt_entr_sh);//cnt_entr_sh=7
	displayEntryShort(&entr_sh[0], cnt_entr_sh, 0); 
	//5. Go to Sub-Folders
	checkFile(&entr_sh[0], cnt_entr_sh);	
}

/*--------------------------- MAIN - END -----------------------------*/


/*********************************************************************/
void callData(){
	//1. BOOT									
	callBootSector();			
	//2. Init Link List
	initList();
	if(checkFAT()!=FAT32){
		
		main_offset();
	
		readDataNode();	
	}else{
		
	}	
}

#endif
