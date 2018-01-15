#include <stdio.h>


int sumOfNaturalNumbers(int number){
	if(number != 0){
		return number + sumOfNaturalNumbers(number-1);
	}
	else{
		return number;	
	}
}


int main(){
	//Will print 0 + 1 + 2 + 3 + 4 + 5
	int zeroToNumber = 5;
	zeroToNumber = sumOfNaturalNumbers(zeroToNumber);
	printf("Summation is %d", zeroToNumber);
	return 1;;
}
