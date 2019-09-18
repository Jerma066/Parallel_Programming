#include <stdlib.h> 
#include <stdio.h>

double **allocate_matrix(int size) {
    double **matrix = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i ++) {
        matrix[i] = (double *)malloc(size * sizeof(double));
    }
    return matrix;
}

double **clean_matrix(int size) {
    double **matrix = allocate_matrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

double **random_matrix(int size) {
    double **matrix = allocate_matrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (double)(rand() % 100000);
        }
    }

    return matrix;
}
