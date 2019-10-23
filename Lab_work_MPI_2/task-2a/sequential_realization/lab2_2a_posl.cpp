#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

#define ISIZE 4
#define JSIZE 4

double f(double x) {
    return sin(0.00001 * x);
}

void output(double a[ISIZE][JSIZE]) {
    FILE *ff;
    ff = fopen("result-variant2A_posl.txt", "w");

    for(int i = 0; i < ISIZE; i++) {
        for (int j = 0; j < JSIZE; j++)
            fprintf(ff, "%f ", a[i][j]);

        fprintf(ff,"\n");
    }

    fclose(ff);
}
	
double a[ISIZE][JSIZE];

int main(int argc, char **argv)
{
	int i, j;
	
	for (i = 0; i < ISIZE; i++){
		for (j = 0; j < JSIZE; j++)
		{
			a[i][j] = 10 * i + j;
		}
	}     
	   
	
    for (size_t i=0; i < ISIZE - 1; ++i)
    {
		for (size_t j = 0; j < JSIZE; ++j)
		{
			if(j != 0){
				a[i][j] = f( a[i+1][j-1] );
			}
		}
	}   
    
	output(a);
}

