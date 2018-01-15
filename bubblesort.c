#import <stdio.h>


void bubbleSort(int *p, int size);

int main(){
	int myarray[] = {7, 4, 1, 2, 35, 0};
	int size = sizeof(myarray) / sizeof(myarray[0]); 
	bubbleSort(myarray, size);
	
	for(int i = 0; i< size; i++){
		printf("%d\n", myarray[i]);
	}
	return 1;
}

void bubbleSort(int *p, int size){
	for(int j = 0; j<size-1; j++){
		for(int i = 0; i<size-1; i++){
		if(*(p+i+1) < *(p+i)){
			int temp = *(p + i + 1);
			*(p+i+1) = *(p + i);
			*(p + i) = temp;
		}
	}
	}
	
}
