const int length = 10, width = +8;
const char firstLowerCase = 'a', firstUpperCase = 'A';

int globalInt, globalIntArray[44];
int globalChar, globalCharArray[44];

int grt(){
	int g;
	g = 8 * 9;
	return (g);
} 
void main(){
	int n ,k;
	n = 0;
	k = 9;
	if(n == 5){
		k = grt();
	}
	else if(n == 4){
		k = 99;
	}
	else if(n == 3){
		k = 88;
	}	
	printf(k);
} 
