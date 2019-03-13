const int length = 10, width = +8;
const char firstLowerCase = 'a', firstUpperCase = 'A';

int globalInt, globalIntArray[44];
int globalChar, globalCharArray[44];
int listInt[100];

void none(int a, int b, int c){
	int i;
	printf(a);
	printf("    ", b);
	printf("    ", c);
	return;
	printf("    ", c);
}
void main(){
	int n ,k, i, c;
	k = 0;
	n = 50;
	c = 20;
	i = 10;
	do{
		listInt[i] = c;
		c = c + 1;
		i = i + 1;
	} while(i!=30);
	for(i = 0; i < 20; i = i + 1)
		k = listInt[i] + k;
		
	k = k / 20 ;
	printf("%d", k);
	printf("             ");
	printf("%d", n);
	printf("             ");
	printf("%d", c);
	printf("             ");
	none(90, 100, 88);
}

