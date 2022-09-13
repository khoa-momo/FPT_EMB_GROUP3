#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>

int main(){
	
	int raw = 0x2a79; 
	
	int mask_day = 0x001f;
	int mask_month = 0x01e0;
	int mask_year = 0xfe00;
	
	int day = (raw & mask_day);
	int moth = (raw & mask_month)>>5;
	int year = ((raw & mask_year)>>9)+1980;
	
	printf("%d\n",moth);
	
	printf("Date(YYYY/MM/DD): %d/%d/%d\n\n",year,moth,day); 


	int raw_time = 0x8f51; 
	
	int mask_sec = 0x001f;
	int mask_min = 0x07e0;
	int mask_hr = 0xf800;
	
	int sec = (raw_time & mask_sec);
	int min = (raw_time & mask_min)>>5;
	int hr = ((raw_time & mask_hr)>11)*2;
	
//	printf("%d\n",moth);
	
	printf("Time(hh:mm:ss): %d:%d:%d",hr,min,sec); 
}

