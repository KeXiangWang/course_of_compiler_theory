const int a = 10, b = 15;
const char add = '+', sub = '-', mul = '*', div= '/';
int BS,bs;
int array[10];
char array_ch[10];

void ptans(int in1, int in2, char op)
{
    int out;
        
    if (op == '+')
        out = in1 + in2;
    if (op == '-')
        out = in1 - in2;
    if (op == '*')
        out = in1 * in2;
    if (op == '/')
        out = in1 / in2;
    printf(" ", out);

}

int fac1(int n)
{
    int f;
    if (n<0)
    {
        printf(" n<0,data error!");
        f = -1;
    }
    if (n == 0)
        f = 1;
    if (n == 1)
        f = 1;
    if (n>1)
        f = fac1(n - 1)*n;
    return(f);
}

void fac2(int in1, int in2)
{
    int i;
    i = 0;
    ptans(in1, in2, add);
    ptans(in1, in2, sub);
    ptans(in1, in2, '*');
    ptans(in1, in2, '/');
    do
    {
        if (in1<in2)
            {  ;  }
        else
            printf(" in1>=in2!");
        in1 = in1 + 1;
        i = i + 1;
    } while (i < 5)
}

int fac3(int j)
{
    int i, temp, sum;
    sum = 0;
    for (i = 0; i < j; i = i + 1)
    {
        temp = fac1(i);
        sum = sum + temp;
    }
    return(sum);
}

int fac4()
{
    const int temp = 5;
    int i;
    for (i = 9; i >= 0; i = i - 1)
    {
        array[i] = temp * ( bs+ BS )+i;        
    }
    return(array[1]);
}


void main()
{
    int i, j, temp;
    int in1, in2;
    int sum;
    scanf(j);
    scanf(BS,bs);  
    
    if (j != 4)
        printf(" j != 4 ");
    sum = fac3(j + 1);
    if (sum)
        printf(sum);
    else
        printf(" wrong!");

    in1 = a + BS;
    in2 = b + bs;
    fac2(in1, in2);
    
    temp = -fac4()+sum*2-bs*(BS+div-'+');
    printf(" this is temp:", temp);
}
