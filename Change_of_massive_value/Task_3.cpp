#include <omp.h>
#include <iostream>
#include <vector>

using namespace std;

int main(){
	int n_threds = 1;
	int n_mass = 16000; 
		
	float a[n_mass];
	float b[n_mass];
	
	float front_vspog[n_threds], back_vspog[n_threds];
	
	omp_set_num_threads(n_threds);
	
	#pragma omp parallel for schedule(static) 
	for(int i = 0; i < 16000; i++){
		a[i] = i;
	}
	
	int k = n_mass/n_threds;
	
	for(int i = 0; i < n_threds; i++){
		front_vspog[i] = a[i*k];
		back_vspog[i] = a[i*k - 1];
	}

	for(int i = 1; i < n_mass - 1; i++)
	{
		b[i] = (a[i-1] + a[i] + a[i+1])/3;
	}
	
	double start, end;
    start = omp_get_wtime();
	
	#pragma omp parallel
	{
		int my_id = omp_get_thread_num();
		int tmp = a[my_id*k];
		#pragma omp barrier	
			
		#pragma omp for schedule(static) 
		for(int i = 1; i < n_mass - 1; i++){
			
			if (i == (my_id + 1) * k - 1){
				a[i] = (tmp + a[i] + front_vspog[my_id+1])/3;
			}
			else if(i == my_id*k){
				a[i] = (back_vspog[my_id] + a[i] + a[i+1])/3;
			}
			else{
				a[i] = (tmp + a[i] + a[i+1])/3;
			}
			
			tmp = a[i];
		} 
	}
	
	end = omp_get_wtime();
	
	cout << end - start << endl;
	
	
	bool answer = true;
	for(int i = 0; i < n_mass - 1; i++){
		if(a[i] != b[i]){
			answer = false;
			cout << "{a: [" << i <<"] = " << a[i] << " ; " << "b["<< i <<"] = "<< b[i] <<"}"<<endl;
		}
	}
	 
	
	
	if(answer){
		cout << "Win :)" << endl;
	}
	else{
		cout << "Lose :(" << endl;
	}
}
