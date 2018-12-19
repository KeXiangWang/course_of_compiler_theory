#include<iostream>
#include<vector> 
using namespace std;

int main(){
	vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8};
	for (auto a = vec.begin(); a != vec.end(); a++){
		if((*a) == 8 || (*a) == 2){
			a = vec.erase(a);
			a --;
		} 
	}
	for (auto a = vec.begin(); a != vec.end(); a++){
		cout<<*a<<endl;
	}
} 
