#include <stdio.h>
#include <math.h>

int toggleBit(int n, int k); 
void printBinary(int n);
void printBinary_toggle(int n);

int main(){
    int n, k;
    printf("Enter n: "); scanf("%d", &n);
    printf("n: %d\n",n);
    do{
    	printf("Enter k (From 1 to 32): "); scanf("%d", &k);
	}while(k<1 || k>32);
    printf("\n");
    
    // Print n & k
    printf("Before the %d-th bit toggled: %d(dec) - ", k, n); 
    //print binary
    printBinary(n);

     
//	int n_toggle = toggleBit(n, k);//flip bit of n
	int n_toggle = (n ^ (1 << (k - 1)));
	
    printf("#After the %d-th bit toggled: %d(dec) - ", k, n_toggle); 
	printBinary_toggle(n_toggle);    
	
    return 0;
} 

// Toggle the kth bit of n
int toggleBit(int n, int k){
	int n_toggle = (n ^ (1 << (k - 1)));
    return n_toggle;
}

void printBinary(int n){
	//Convert int n (decimal) -> int[] (binary) || for printing binary  
    int bin[32];    
    int i; 
	for(i=0; n>0; i++){    
		bin[i] = n%2;    
		n = n/2;    
	}        
	//Print array of binary     
	for(i=i-1; i>=0; i--){  
		printf("%d", bin[i]);    
	}
	printf("(bin)\n");   
}

void printBinary_toggle(int n_toggle){
	//Convert int n (decimal) after toggle -> int[] (binary) || for printing binary
	int bin_toggle[32];    
	int i;
	for(i=0; n_toggle>0; i++){    
		bin_toggle[i] = n_toggle%2;    
		n_toggle = n_toggle/2;    
	}    	
	//Print array of binary  
	for(i=i-1; i>=0; i--){  
		printf("%d",bin_toggle[i]);    
	}
	printf("(bin)\n");
}
 
 

