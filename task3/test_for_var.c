const int length = 10, width = +8;
const char firstLowerCase = 'a', firstUpperCase = 'A';

int globalInt, globalIntArray[44];
int globalChar, globalCharArray[44];


int none(int a, int b, int c){
	const int testGlobal1 = 5;
	const int testGlobal2 = 5;
	const int testGlobal3 = 5;
	const int testGlobal4 = 5;
	const int testGlobal5 = 5;
	const int testGlobal6 = 5;
	const int testGlobal7 = 5;
	const int testGlobal8 = 5;
	const int testGlobal9 = 5;
	const char testddddd10 = 'd';
	int d, e, f;
	char g, h, k;
	d = 0;
	e = 1;
	f = 2;
	g = 3;
	h = 4;
	k = 5;
	k = a + b + c + d + e + f + g + h + k;
	return (k);
}
void main(){
	int n ,k;
	n = 5;
	k = 6;
	k = n * k;
	n = none();
	printf(k);
	printf(n);
} 
