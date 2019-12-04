#include <iostream>
#include <omp.h>
#include <fstream>

using namespace std;

void qSort_parallel_internal(int* array, int left, int right, int cutoff)
{

    int i = left, j = right;
    int tmp;
    int median = array[(left + right) / 2];

    while (i <= j) {
        while (array[i] < median){
            i++;
        }
        while (array[j] > median) {
            j--;
        }
        if (i <= j) {
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            i++;
            j--;
        }
    }


    if ( ((right-left)<cutoff) ){
        if (left < j){
            qSort_parallel_internal(array, left, j, cutoff);
        }
        if (i < right){
            qSort_parallel_internal(array, i, right, cutoff);
        }

    }
    else{
        #pragma omp task
        {
            qSort_parallel_internal(array, left, j, cutoff);
        }
        #pragma omp task
        {
            qSort_parallel_internal(array, i, right, cutoff);
        }
    }
}

void qSort_parallel(int* array, int lenArray, int numThreads, int cutoff){
    #pragma omp parallel num_threads(numThreads)
    {
        #pragma omp single nowait
        {
            qSort_parallel_internal(array, 0, lenArray - 1, cutoff);
        }
    }
}

int main(int argc, char* argv[]) {
    int numThreads = atoi(argv[1]);
    int numOfElements = 1000;

    //Запись ммассива в файл
    ofstream output("data.txt");
    for (int j = 0; j < numOfElements; ++j) {
        output << rand() % 150;
        output << ' ';
    }


    //Подгрузка массива из файла
    ifstream input("data.txt");

    if(input.is_open()) {
        int mass[numOfElements];
        for(int i = 0; i < numOfElements; i++) {
            input >> mass[i];
        }

        int cutoff = (numOfElements / numThreads);
        //int cutoff = 2000;

        double start_time = omp_get_wtime();
        qSort_parallel(mass, numOfElements, numThreads, cutoff);
        double end_time = omp_get_wtime();

        cout << "{Time: " << end_time - start_time << "}" << endl;

/*
        for(int i = 0; i < numOfElements; i++){
            cout << mass[i] << " ";
        }
*/
    }
    else{
        cout << "Error of reading file :(" << endl;
    };

    return 0;
}