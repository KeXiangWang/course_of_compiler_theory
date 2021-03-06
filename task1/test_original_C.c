#include<stdio.h>

const int length = 10, width = +8;
const char firstLowerCase = 'a', firstUpperCase = 'A';
const char lastLowerCase = 'z', lastUpperCase = 'Z';

int globalInt;

int fib(int n, int k){
	int i;
	int res;
	if(n <= k){
		return (1);
	}
	else{
		res = 0;
		for(i = n - k; i < n; i = i + 1){
			res = res + fib(i, k);
		}
		return (res);
	}
}

void print(){
	char i;
	i = 32;
	do{
		if(i != 34){
			printf("%c", i);
			// printf(i);
		}
		i = i + 1;
	} while(i < 127);
	// } while(i < 127)
}

char toUpperCase(char x)
{
    char c;
    if (x >= firstLowerCase)
        if (x <= lastLowerCase)
        {
            c = x - firstLowerCase + firstUpperCase;
            return(c);
        }
        else return(x);
    else return(x);
}

void changeGlobal(){
	printf("original: ");
	printf("%d", globalInt);
	// printf(globalInt);
	globalInt = 16;
	printf("   changed: ");
	printf("%d", globalInt);
	// printf(globalInt);
}

void dealRequest(int operation){
	const char x = 'x';
	int i;
	char y;
	printf("start  ");
	y = 'y';
	if(operation == 0){
		for(i = 1;i <= length;i = i + 1){
			printf(" %d", fib(i, 2));
			// printf(" ", fib(i, --2)); 
		}
	} else if(operation == 1){
		print();
	} else if(operation == 2){
		printf("%c", toUpperCase(x));
		printf("%c", toUpperCase(y));
		// printf(toUpperCase('x'));
	} else if(operation == 3){
		changeGlobal();
	} else {
		printf("nothing");
	}
}

int main(){
// void main(){ 
	int i;
	int operation;
	globalInt = 0;
	scanf("%d", &operation);
	// scanf(operation);
	
	for(i = 0; i < operation; i = i + 1){
		dealRequest(i);
		printf("            ");
	}
}
