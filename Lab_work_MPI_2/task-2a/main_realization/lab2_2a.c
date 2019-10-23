#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ISIZE 990
#define JSIZE 990

double f(double x) {
    return sin(0.00001 * x);
}	

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int id;
	int nthreads;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);				//получаем id процесса в котором находимся
	MPI_Comm_size(MPI_COMM_WORLD, &nthreads);		//получаем общее количество процессов 
	
	double a[ISIZE][JSIZE];
	
	int i, j;
	
	//Из условия варианта без изменений
	for(i = 0; i < ISIZE; i++)
	{
		for(j = 0; j < JSIZE; j++)
		{
			a[i][j] = 10*i + j;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
//---------Блок расчетов----------------------------------------------// 
	//фиксируем время начала работы цикла
    float start_time = MPI_Wtime();
	
	int start_index =  id    * (ISIZE / nthreads);		//получение начального идекса в массиве для каждого процесса
	int end_index   = (id+1) * (ISIZE / nthreads);		//получение конечного идекса в массиве для каждого процесса
	int extra =  ISIZE % nthreads;						//остаток после деления массива по потоком
	
	//Перераспределение extra индексов
	//Перераспределение - равмернное.
	if(id < extra){
		start_index +=  id;				
		end_index	+= (id + 1);		
	}
	else{
		start_index += extra;
		end_index   += extra;
	}
	
	// Распаралеливание по внешниму циклу, за счет деления итераций по i (строкам) в каждом потоке от 
	// start_index до end_index
	for(i = start_index; i < end_index; i++) 
	{
		if(i < ISIZE - 1){
			for(j = 1; j < JSIZE; j++)
			{
				a[i][j] = f( a[i+1][j-1] );
			}
		}
	}

	//Заполнение элементов необхоимых для MPI_Gatherv
	double* send_buf  = &a[start_index][0];				//указатель на адрес начального идекса в матрице каждого потока
	double* recv_buf  = &a[0][0];						//адрес начала исходного массива
	long send_size = JSIZE*(end_index - start_index);
	
	int	recv_size[nthreads];	//Параметр MPI_Gatherv - размеры получаемых данных - это массив, элементы которого - размеры выделяемые в матрице a для почения даннных
	int displs[nthreads];		//Параметр MPI_Gatherv - позиции получаемых данных - это массив, элементы которого - позиции в матрице а, начиная с которых будут вписываться полученные даннные
	
	displs[0] = 0;
	recv_size[0] = JSIZE*(end_index - start_index);
	
	for(i = 1; i < nthreads; i++)
	{
	   	displs[i] = displs[i-1] + (i <= extra  ?  JSIZE*(ISIZE/nthreads + 1)  :  JSIZE*(ISIZE/nthreads) );
		recv_size[i] = (i < extra  ?  JSIZE*(ISIZE/nthreads + 1)  :  JSIZE*(ISIZE/nthreads) );
	} 	
	
	MPI_Gatherv(send_buf, send_size, MPI_DOUBLE, recv_buf, recv_size, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	//фиксируем время окончания работы цикла
	float end_time = MPI_Wtime();
    float time = end_time - start_time;

	MPI_Finalize();		
//--------------------------------------------------------------------//
	
	if(id == 0)
	{
		printf("time = %.20f\n", time);
		
		//Запись в файл:
		FILE *ff;
		ff = fopen("result-variant2A.txt", "w");
		for(i = 0; i < ISIZE; i++)
		{
			for(j = 0; j < JSIZE; j++)
				fprintf(ff, "%f ", a[i][j]);
		
			fprintf(ff, "\n");
		}
		
		fclose(ff);
		
		FILE *analysis;
		analysis = fopen("result_for_analysis_second_realization.txt", "a");
		fprintf(analysis, "{ nthreads = %d ", nthreads);
		fprintf(analysis, "; time = %.20f }", time);
		fprintf(analysis,"\n");
		fclose(analysis);
	}
	
	return 0;
}

