#include <iostream>
#include <omp.h>

using namespace std;

int main(){
	int i;
	int sum = 0;
	int N = 10;
	
	#pragma omp parallel for if(N>4) private(i) reduction(+:sum) shared(N) schedule(static) 
		for(i = 0; i < N+1; i++)
			sum += i;
	
	cout << sum << endl;
	
	return 0;	
}
