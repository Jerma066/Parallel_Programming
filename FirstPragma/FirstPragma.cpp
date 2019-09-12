#include <iostream>
#include <omp.h>

using namespace std;

int main(){
	int counter;
	
	#pragma omp parallel
	{
		int myid = omp_get_thread_num();
		
		if(myid == 0){	
			counter	= omp_get_num_threads();
			cout << "Number of threads: " << counter << endl;
			counter--;
		}
		
		while(myid != counter);
		
		cout << myid << endl;
		counter--;
	}
	
	return 0;	
}

