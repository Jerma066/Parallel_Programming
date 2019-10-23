#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

#define ISIZE 600
#define JSIZE 600

double f(double x) {
    return sin(0.00001 * x);
}	

void size_of_data(double& size, const int& id,const int& nthreads){
	if(id == nthreads - 1){
		size = (ISIZE/nthreads)*(JSIZE - 1) - (JSIZE - 1);
	}
	else{
		size = (ISIZE/nthreads)*(JSIZE - 1);
	}
}

int main(int argc, char **argv)
{	
	double a[ISIZE][JSIZE];
	double mass[ISIZE*JSIZE];
	
	int b;

	int id, nthreads;
	int i, j;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &nthreads); 
	
	for (i = 0; i < ISIZE; i++)
	{
		for (j = 0; j < JSIZE; j++)
		{
			a[i][j] = 10 * i + j;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);    

    //Блок расчетов  
    auto start_time = MPI_Wtime();
	
	double data_vspog[(ISIZE/nthreads)*JSIZE];
	long k = 0;
    for (size_t i = (ISIZE/nthreads)*id; i < (ISIZE/nthreads)*(id+1); ++i)
    {
		for (size_t j = 1; j < JSIZE; ++j)
		{
			if(id == 0){
				mass[k] = f( a[i+1][j-1] );
				k++;
			}
			else{
				if(i != ISIZE ){
					data_vspog[k] = f( a[i+1][j-1] );
					k++;
				}
			}
		}
	}
		
	if(id != 0){
		double size;
		size_of_data(size, id, nthreads);
		
		MPI_Send(data_vspog, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else{
		for(int i = 1; i < nthreads; i++){
			double size;
			size_of_data(size, i, nthreads);
		
			MPI_Recv(&mass[i*(ISIZE/nthreads) * (JSIZE - 1)], size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, NULL);
		}
	}
	
	
    auto end_time = MPI_Wtime();
    auto time = end_time - start_time;
    
    if(id == 0) {
		for(size_t i = 0; i < ISIZE - 1; i++)
		{
			for(size_t j = 1; j < JSIZE; j++) 
			{
				a[i][j] = mass[i*(ISIZE-1) + j - 1];
			}
		}
	}
    
	if (id == 0) {
		cout << "time : " << setprecision(20) << time << std::endl;
		
		//Запись в файл:
		FILE *ff;
		ff = fopen("result-variant2A.txt", "w");

		for(int i = 0; i < ISIZE; i++) {
			for (int j = 0; j < JSIZE; j++)
				fprintf(ff, "%f ", a[i][j]);

			fprintf(ff,"\n");
		}
		fclose(ff);	
		
		FILE *analysis;
		analysis = fopen("result_for_analysis_second_realization.txt", "a");
		fprintf(analysis, "{ nthreads = %d ", nthreads);
		fprintf(analysis, "; time = %.20f }", time);
		fprintf(analysis,"\n");
		fclose(analysis);
		
	}	
	MPI_Finalize(); 
}

