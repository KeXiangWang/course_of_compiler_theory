const int length = 10, width = +8;
const char firstLowerCase = 'a', firstUpperCase = 'A';

int globalInt, globalIntArray[44];
int globalChar, globalCharArray[44];
int listInt[100];

int none(int a, int b, int c){
	int i;
	for(i = a; i < b; i = i + 1){
		listInt[i] = c;
		c = c + 1;
	} 
	return (c);
}
void main(){
	int n ,k, i;
	k = 0;
	n = none(0, 20, 20);
	for(i = 0; i < 20; i = i + 1)
		k = listInt[i] + k;
	k = k / 20 ;
	printf("%d", k);
	printf("             ");
	printf("%d", n);
} 
