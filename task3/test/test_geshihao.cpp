//#include<stdio.h>
const char multi='+';
const int one=1;
const int two=2;
const int three=3;
const int four=4;
const int five=5;
const int One=10;
const int Two=20;
const int Three=30;
const int Four=40;
const int Five=50;
int small_array[10];
int large_array[100];
char mul;
void pro_start()
{
     int i;
     i=0;
    do
    {
        small_array[i]=four;
        i=i+1;
    }
    while(i!=One);
    small_array[10]='a';
    for(i=0;i<11;i=i+1)
    {
        printf("%d ",small_array[i]);
        printf("\n");
    }
    printf("prepare finished.\n");
}
int init_array(int number)
{
    if(number<=5)
    {
        number=number+1; 
        return(init_array(number));
    }
    else
    {
        return(number);
    }
}
void save_in_array(char ch)
{
    int i,j,location;
    for(i=0;i<5;i=i+1)
    {
        for(j=0;j<=5;j=j+1)
        {
            large_array[i+j]=ch;
        }
    }
    for(i=10;i<51;i=i+1)
    {
        large_array[i]=ch;
        ch=ch+1;
    }
    printf("%d ",large_array[10]);
    printf("\n");
    printf("%d ",large_array[50]);
    printf("\n");
}
void main()
{
    const char minus='-';
    char ch;
    int number;
    int result;
    mul=multi;
    ch='a';
    if(mul=='+'){}
    else
    {
        return;
    }
    pro_start();
    printf("PROGRAM START!!!\n");
    printf("your lucky number is:",1);
    printf("\n");
    printf("choose your branch(from number 1 to 2):\n");
    scanf(number);
    if(number!=0)
    {
        result=init_array(number);
    }
    printf("res:%d ",result);
    result=+1-1+1-1*(19999+29999-10000*(result/result*(two-1*(Two-19))))+result;
    printf("res:%d ",result);
    ch=ch+one;
    save_in_array(ch);
    printf("res:%d ",result);
    return;
} 
