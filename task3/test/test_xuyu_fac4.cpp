int array[10];
int bs, BS;
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

void main(){
	bs = 3;
	BS = 2;
	printf("%d", fac4());
} 
