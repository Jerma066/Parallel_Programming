#include <iostream>
#include <omp.h>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]){
	int i;
	long int sum = 0;
	int N = atoi(argv[1]);
	
	#pragma omp parallel for if(N>4) private(i) reduction(+:sum) shared(N) schedule(static) 
		for(i = 0; i < N+1; i++)
			sum += i;
	
	cout << sum << endl;
	
	return 0;	
}

