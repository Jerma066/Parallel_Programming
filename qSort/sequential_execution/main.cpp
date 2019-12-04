#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

void qSort_internal(int* array, int left, int right)
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


    if (left < j){
        qSort_internal(array, left, j);
    }
    if (i < right){
        qSort_internal(array, i, right);
    }
}

void qSort(int* array, int lenArray){
    qSort_internal(array, 0, lenArray - 1);
}

int main(int argc, char* argv[]) {
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

        double start_time = omp_get_wtime();
        qSort(mass, numOfElements);
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