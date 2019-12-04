#include <stdio.h>
#include <omp.h>

#define NUM_OF_THREADS 2

int main(int argc, char* argv[])
{
	int u = 0;
	int num_of_parts = atoi(argv[1]);
	
	while(u < 100){
		omp_set_num_threads(NUM_OF_THREADS);
	
		int i;
		long double sum[NUM_OF_THREADS];
		long double res = 0.0;
		double start_time, end_time;
		
		start_time = omp_get_wtime();
		//--------------Первый этап формирования суммы ряда----------------------------------//
		#pragma omp parallel
		{
			int myid = omp_get_thread_num();
			sum[myid] = 0;
		
		
			//После выполнения каждый элемент массива будет иметь результат равынй:
			//1/((k+d)*(k+d-1)*...*(k)) + 1/((k+d-1)*(k+d-2)*...*(k)) + ... + 1/k, 
			//где (k, k+d+1) - участок, который достался исполнителю с сооотв. id.
			#pragma omp for schedule(static) private(i)
			for(i = num_of_parts; i >= 1; i--)
			{	
				sum[myid] = (sum[myid] / (long double)i) + (1 / (long double)i);
			}
			#pragma omp barrier
		}
		//-----------------------------------------------------------------------------------//
		//--------------Второй этап формирования суммы ряда----------------------------------//
		int j;
		for(j = 0; j < NUM_OF_THREADS; j++)
		{
			//offset = k для если речь также идет о участоке (k, k+d+1)
			int offset = (NUM_OF_THREADS - 1 - j) * num_of_parts / NUM_OF_THREADS;
			
			long double current = sum[j];
			
			//параллельная реализация домножения на 1/(offset!)
			#pragma omp parallel reduction(*: current)
			{
				#pragma omp for private(i) 
				for(i = offset; i >= 1; i--)
				{	
					current *= (1 / (long double)i);
				}
			}
			
			sum[j] = current;
		}
		//-----------------------------------------------------------------------------------//
		//--------------Третий этап формирования суммы ряда----------------------------------//
		for(i = 0; i < NUM_OF_THREADS; i++)
			res += sum[i];
		//-----------------------------------------------------------------------------------//
		end_time = omp_get_wtime();
		
		printf("{res: %.20Lf;  time = %.10f}\n", res + 1, end_time - start_time);
		u++;
	}
	
	return 0;
}
