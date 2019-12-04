/******************************************************************************
* ЗАДАНИЕ: bugged6.c
* ОПИСАНИЕ:
*   Множественные ошибки компиляции
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

float dotprod()
{
    int i, tid;
    float sum = 0.0;

    tid = omp_get_thread_num();
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < VECLEN; i++)
    {
        sum = sum + (a[i] * b[i]);
        printf("  tid= %d i=%d\n", tid, i);
    }
    
    return sum;
}


int main (int argc, char *argv[])
{
    int i;
    float sum;

    for (i = 0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;
    sum = 0.0;
    
    /*
     * Допишем в функции return sum
     * Уберем shared перед вызовом функции добавим 
     * parallel перед распараллелеиваемым for-ом
     */ 
    sum = dotprod();

    printf("Sum = %f\n",sum);
}
