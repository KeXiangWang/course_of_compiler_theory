#include<stdio.h>
int er;
int add(int a, int b){
	er = 6;
	return (a+b);
	
}
int main(){
	int a;
	int b;
	int c,d,e;
	er = 5;
	a = 3;
	b = 4 + er;
	c = add(a, a);
	d = 4 + er;
	printf("%d", c);
	printf("%d", d);
	
}
