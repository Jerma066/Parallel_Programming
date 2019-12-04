/******************************************************************************
* ЗАДАНИЕ: bugged4.c
* ОПИСАНИЕ:
*   Очень простая программа с segmentation fault.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define N 100
/*
 * Изменение количеств аэлементов
 * спасло эту программу
 */ 

int main (int argc, char *argv[]) 
{
    int nthreads, tid, i, j;
    double a[N][N];
    
    /*
     * a = (double**)malloc(N * sizeof(double*));
     * for(int i = 0; i < N; i++){
     * 		a[i] = (double*)malloc(N * sizeof(double*));
     * }
     */ 

    #pragma omp parallel shared(nthreads) private(i, j, tid, a)
    {
       tid = omp_get_thread_num();
       if (tid == 0) 
       {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
       }
       printf("Thread %d starting...\n", tid);      

        for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
                a[i][j] = tid + i + j;

        printf("Thread %d done. Last element= %f\n", tid, a[N-1][N-1]);
    } 
}
