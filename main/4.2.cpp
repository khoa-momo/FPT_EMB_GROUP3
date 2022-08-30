#include <stdio.h>
#include <math.h>
 
int ADD(int a, int b);
int OR(int a, int b); 
int XOR(int a, int b); 
int Bin2Dec(int bin_c[], int length);
int processing(int a, int b, int type);

int main(){ 
    int a, b;
    printf("Nhap a = ");scanf("%d", &a);
    printf("Nhap b = ");scanf("%d", &b);
    
    printf("\nADD(%d,%d): %d",a,b,ADD(a,b));
    printf("\nOR(%d,%d): %d",a,b,OR(a,b));
    printf("\nXOR(%d,%d): %d",a,b,XOR(a,b));
    return 0;
}

int ADD(int a, int b){   
 	return processing(a, b, 1); 
}

int OR(int a, int b){   
    return processing(a, b, 2);
}
 
int XOR(int a, int b){  
	return processing(a, b, 3);
}
 
int processing(int a, int b, int type){
	// 1.convert 1st number to binary
    int bin_a[32],i;   
	int count_a = 0;
	for(i=0; a>0; i++){    
		bin_a[i] = a%2;    
		a=a/2;    
	}    
	//printf("\na: ");	
	for(i=i-1; i>=0; i--){    
		//printf("%d",bin_a[i]);    
		count_a++; 
	}    
	//printf("\ncount of bina[]: %d",count_a);
	 
	// 2.convert 2nd number to binary
    int bin_b[32],j;   
	int count_b=0;
	for(j=0; b>0; j++){    
		bin_b[j] = b%2;    
		b=b/2;    
	}    
	//printf("\nb: ");	
	for(j=j-1; j>=0; j--){    
		//printf("%d",bin_b[j]);    
		count_b++; 
	}    
	//printf("\ncount of binb[]: %d\n",count_b);
 	 
	//3. Fill 0 bits for the number have less digit(binary)
 	int length = count_a;   
	if(count_a<count_b){ //fill 0 for a
		//printf("\ncount_a<count_b\n\n"); 
		for(int i=count_a; i<=count_b-1; i++){ 
			bin_a[i] = 0;
		}
 		length = count_b;//get the longer one
	}
	if(count_b<count_a){ //fill 0 for b
		//printf("\ncount_b<count_a\n\n"); 
		for(int i=count_b; i<=count_a-1; i++){
			bin_b[i] = 0;
		}
	}
	//printf("\nlength:%d \n\n",length); 		
 
 	//Process AND, OR, XOR function
	int bin_c[32];
	if(type==1){
		//AND 2 numbers using forloop and save to an arr int
		for(int k=0; k<length; k++){
			//printf("%d ",bin_a[k]);  
			//printf("%d ",bin_b[k]);    
			//printf("||");
			//AND
			bin_c[k] = 1;  
			if(bin_a[k]==0 || bin_b[k]==0){
				bin_c[k] = 0;  
			} 
		}  		
	}
	else if(type==2){
		//OR 2 numbers using forloop and save to an arr int
		for(int k=0; k<length; k++){
			//printf("%d ",bin_a[k]);  
			//printf("%d ",bin_b[k]);    
			//printf("||");
			//OR
			bin_c[k] = 1;  
			if(bin_a[k]==0 && bin_b[k]==0){
				bin_c[k] = 0;  
			} 
		}    
	}else{ 
		//XOR 2 numbers using forloop and save to an arr int
		for(int k=0; k<length; k++){
			//printf("%d ",bin_a[k]);  
			//printf("%d ",bin_b[k]);    
			//printf("||"); 
			//XOR
			bin_c[k] = 1;  
			if(bin_a[k]==bin_b[k]){
				bin_c[k] = 0;  
			} 
		}
	}
	//4. Convert arr int of binary back to decimal
 	return Bin2Dec(bin_c, length);
}
  
int Bin2Dec(int bin_c[], int length){
	int p = 0;
    int decNumber = 0;
	
	//printf("\n\nc: ");
	for(int k=length-1; k>=0; k--){    
		//printf("%d",bin_c[k]);     
	}   
	 
    for(int k=0; k<length; k++){
    	decNumber += (bin_c[k] % 10) * pow(2, p);
        ++p; 
	}
    return decNumber;
}
