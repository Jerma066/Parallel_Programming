#include <omp.h>
#include <time.h>
#include "matrix.h"

int main(){

    int size = 1000;
    int n = 4; 			//threads count


    printf("Matrix size %d  \n", size);   
    printf("Using  %d threads \n",n);


    double **A = random_matrix(size);
    double **B = random_matrix(size);
    double **C = clean_matrix(size);


    double start, end;
    start = omp_get_wtime();


    //clock_t start = clock();
    omp_set_num_threads(n);

    #pragma omp parallel for schedule(dynamic) 
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			double sum = 0;
			for (int k = 0; k < size; k++) {
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}
	
	
	
	

    end = omp_get_wtime();
    printf("Accelerate Runtime is %f seconds \n", end-start);
    
    
    
    //~~~Для проверки ускорения~~~~~~~~~~~~~~~~~~~~~~~~~
    double **D = clean_matrix(size);
    
    start = omp_get_wtime();
    
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			double sum = 0;
			for (int k = 0; k < size; k++) {
				sum += A[i][k] * B[k][j];
			}
			D[i][j] = sum;
		}
	}
	
	end = omp_get_wtime();
	
	printf("Real Runtime is %f seconds \n", end-start);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    

    return 0;
}

