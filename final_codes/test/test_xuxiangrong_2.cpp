#include<stdio.h>
int b[100];
char c,d,e[100];

int testfac(int n){
	if(n==0){
		return(1);
	}else{
		return(testfac(n-1)*n);
	}
}

void test(){
	const int ca=1,cb=2;
	int inta,intb;
	int a[100];
	int c[10];
	a[0]=4;
	a[1]=0;
	a[6]=3;
	a[testfac(a[testfac(0)])]=testfac(a[testfac(a[testfac(a[6])])]);
	printf("%d", a[9-2*4]);
}


int main(){
	test();
}
