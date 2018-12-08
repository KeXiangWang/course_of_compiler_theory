#include<stdio.h>

const int pai = 3,   _e = 2;  const char c = 'c', sdf = '2',  ddd = 'd';
int _sfd, ary11[20],   jfi321;
char s123, char23[23], jf320, sxmri31[10];

int factorial (int n){
    if (n == 2){
        return (2);
    }
    else {
        return (n*factorial(n-1));
    }
}


void output(){
    int counter, sum;
    counter = pai;
    sum = 0;
    while (counter>0){
        printf("char c = %c ",c);
        printf(",count down ",counter);
        counter = counter-1;
        sum = sum + c;
    }
    printf("c times pai is ",sum);
    return;
}


char printchar(int n){
    char ca;
    ca = n;
    return (ca);
}


int main(){
    int ary[5];
    int var_1, var_2, i, suma;
    char char_1, char_2;
    printf("var_1 = ");
    scanf("%d",var_1);
    printf("var_2 = ");
    scanf("%d",var_2);
    printf("char_1 = ");
    scanf("%d",char_1);
    printf("char_2 = ");
    scanf("%d",char_2);
    suma = 0;
    while(var_2!=5){
        if (var_2<5) var_2 = var_2+1;
        else var_2 = var_2-1;
    }

    for (i=10;i<15;i=i+1){
        ary[i-10] = var_2 + i;
        suma = suma + ary[i-10];
    }

    printf("Hello world!");
    var_2 = (var_2*(3+1)-1*2)/6;

    if (var_1>0){
        printf("var_1 is bigger than 0");
    }
    if (var_1>=0){
        printf("var_1 beq 0");
    }
    if (var_1<0){
        printf("var_1 is smaller than 0");
    }
    if (var_1<=0){
        printf("var_1 leq than 0");
    }
    if (var_1==0){
        printf("var_1 equals 0");
    }
    if (var_1!=0){
        printf("var_1 neq 0");
    }
    printf("%d", var_2);
    printf("sum of ary is ", suma);
    printf("factorial of 3 is ",factorial(var_2));
    printf("char_1 is ",char_1);
    printf(",char_2 is ",char_2);
    printf(",sum of char_1 and char_2 is ",char_1+char_2);
    output();
    printf("ascii no.",70);
    printf(" is ",printchar(70));
} 
