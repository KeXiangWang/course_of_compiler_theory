#include<stdio.h>

const int MAX_STACK_NUM = 0010, MAX_CHOOSE_NUM = 10;    
const char DIVIDE = '_';

int stack[12], used[12];    
int n, m;
int sum;

void permutations(int a)    
{                        
    int i;            
    i = 0;

    if (a == m)
    {
        
        do {
            printf("%d ", stack[i]);
            i = i + 1;
        }while (i < m);
        printf("___________");
        return;
    }

    for (i = 1; i <= n; i = i + 1)
    {
        if (used[i] != 0)
            ;
        else
        {
            stack[a] = i;
            used[i] = 1;
            permutations(a + 1);
            used[i] = 0;
        }
    }
}

int get_max_num_1()            
{                            
    return (MAX_STACK_NUM);    
}

int get_max_num_2()
{
    int max_num;
    max_num = MAX_CHOOSE_NUM;
    return (max_num);
}

int get_2()
{
    const int m = 2;
    return (m);
}

int compute(int a, int b, char op)    
{
    if (op == '+')
        return ((a + 2 * b) - b);
    else
    {
        if (op == '-')
            return (a - (b + b) / 2);
        else
        {
            if (op == '*')
                return (a * b);
            else
            {
                if (b)
                    return (a / b);
                else
                    return (((1+2)*3-4)/5*6 + -7 + 1);
            }
        }
    }
}

char get_permutations_sy()    
{
    const char PERMUTATIONS_SY = 'T';    
    return (PERMUTATIONS_SY);     
}

char judge(int n, int m)
{
    const char DIVIDE = '-';
    printf("%d", DIVIDE);
    if (n >= get_max_num_1 + 1)
        return ('F');
    if (m > n)
        return ('F');
    if (m <= 0)
        return ('F');
    return ('T');
}

void preparation()    
{                    
    const int zero = 0;
    int n;
    n = compute(get_max_num_1, 1, '+');
    
    do{
        n = -1 + n;
        used[n] = zero;
    }while (n);
    for (n = compute(MAX_CHOOSE_NUM, get_2, '+') - 1; n; n = n - 1)
        stack[n - 1] = zero;
    return;
}

void test_for()
{
    int i;
    for(i = 1; 0; i = i + 1)
        printf("test for success");
}

int test_6_para(int a1,int a2,int a3,int a4,int a5,int a6)
{
    return (a1+a2+a3+a4+a5+a6);
}

void main()    
{
    int a, b;
    char op;
    char sy,nor_sy;

    test_for;
    scanf(A, b, op);
    n = compute(a, b, op);
    printf(n);
    printf(DIVIDE);
    
    scanf(m);
    sy = judge(n, m);
    sum = test_6_para(a,b,n,m,1,2);
    printf(sum);

    printf("You can choose m numbers permutations from n numbers ? ", sy);
    printf("___________");

    nor_sy = get_permutations_sy();

    if ((sy) != (nor_sy))
        printf("error input: 0 < m <= n <= 10");
    else
    {
        preparation();
        permutations(0);
    }
