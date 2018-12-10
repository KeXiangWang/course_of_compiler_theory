const int length = 10, width = +8;
const char firstLowerCase = 'a', firstUpperCase = 'A';
const char lastLowerCase = 'z', lastUpperCase = 'Z';

int globalInt, globalIntArray[44], globalInt2, globalIntArray2[66], globalInt3;
int globalChar, globalCharArray[44], globalChar2, globalCharArray2[66], globalChar3;
int n;

int fib_1(int n, int k){
	int i;
	int res;
	if(n <= k){
		return (1);
	}
	else{
		res = 0;
		for(i = n - k; i < n; i = i + 1){
			res = res + fib_1(i, k);
		}
		return (res);
	}
}

void print(){
	char i;
	i = 32;
	do{
		if(i != 34){
			printf(i);
		}
		i = i + 1;
	} while(i < 127)
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
	printf(globalInt);
	globalInt = 16;
	printf("   changed: ");
	printf(globalInt);
}

void dealRequest(int operation){
	const char x = 'x';
	int i;
	char y;
	printf("start  ");
	y = 'y';
	if(operation == 0){
		for(i = 1;i <= length;i = i + 1){
			printf(" ", fib_1(i, --2)); 
		}
	} else if(operation == 1){
		print();
	} else if(operation == 2){
		printf("%c", toUpperCase(x));
		printf(toUpperCase('x'));
	} else if(operation == 3){
		changeGlobal();
	} else {
		printf("nothing");
	}
}


int fib_2(int n){
	if(n <= 0){
		return (1);
	}
	else{
		return (n*fib_2(n-1)); 
	}
}
int max(int a, int b){
	if(a > b){
		return (a);
	}
	else{
		return (b);
	} 
} 
void main(){ 
	int i;
	int n;
	i = 0;
	do{
		printf(i);
		printf("    ");
		i = i + 1; 
		n = max(5,2);
	} while(i<5)
	printf(fib_2(n));
}
