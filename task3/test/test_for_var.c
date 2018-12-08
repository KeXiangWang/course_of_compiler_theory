const int length = 10, width = +8;
const char firstLowerCase = 'a', firstUpperCase = 'A';

int globalInt, globalIntArray[44];
int globalChar, globalCharArray[44];


int none(int a, int b, int c){
	const int testGlobal1 = 50000;
	const int testGlobal2 = 50000;
	const int testGlobal3 = 50000;
	const int testGlobal4 = 5;
	const int testGlobal5 = 5;
	const int testGlobal6 = 5;
	const int testGlobal7 = 5;
	const int testGlobal8 = 5;
	const int testGlobal9 = 5;
	const char testddddd10 = 'd';
	int d, e, f, p, q, w, x, y, z;
	char g, h, k;
	d = 0;
	e = 1;
	f = 2;
	g = 3;
	h = 4;
	k = 5;
	p = 6;
	q = 7;
	w = 8;
	x = 9; 
	y = 10;
	z = 11;
	k = a + b + c + d + e + f + g + h + k + p + q + w + x + y + z + testGlobal1 + testGlobal2 + testGlobal3;
	return (k);
}
void main(){
	int n ,k;
	n = 5;
	k = 6;
	k = n * k;
	n = none(100, 100, 100);
	printf(k);
	printf(n);
} 
