int x;
int add() {
    x = x+1;
    return (x);
}

int sum(int a, int b, int c, int d) {
    return (a+b+c+d);
}

void main() {
    x = 1 + 0; 
    x = 1 + 0; 
    printf(sum(x, add(), x, add()));
    x = x + 1;
	printf(x); 
}
