#include <iostream>
#include <omp.h>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])
{
	int lastNumber = atoi(argv[1]);
	int num_of_threads = atoi(argv[2]); 
	
	omp_set_num_threads(num_of_threads);
	
	//Число до которого мы будем идти выбирая элемент, кратные которому будем занулять
	const int lastNumberSqrt = (int)sqrt((double)lastNumber);
	//Все четные числа(кроме двойки) - непростые. Так что замер массива значений будет равен
	int memorySize = (lastNumber-1)/2;				
	
	double start_time = omp_get_wtime();						
	
	//----Инициализаци и параллельное заполнение решета Эратосфена-----------------------------//
	char* isPrime = new char[memorySize+1];
	#pragma omp parallel for
		for (int i = 0; i <= memorySize; i++){
			isPrime[i] = 1;
		}
	//-----------------------------------------------------------------------------------------//
	//----Зануление всех непросттых позиций(=чисел) в решете-----------------------------------//
 	#pragma omp parallel for schedule(dynamic)
		for (int i = 3; i <= lastNumberSqrt; i += 2)
		{
			if (isPrime[i/2])
			{
				for (int j = i*i; j <= lastNumber; j += 2*i)
				{
					isPrime[j/2] = 0;
				}
			}
		}
	//-----------------------------------------------------------------------------------------//  
	//----Подсчет количества простых чисел-----------------------------------------------------//
	int found = lastNumber >= 2 ? 1 : 0;	
	
	#pragma omp parallel for reduction(+:found)
		for (int i = 1; i <= memorySize; i++)
		{
			found += isPrime[i];
		}
	//-----------------------------------------------------------------------------------------//
	
	double end_time = omp_get_wtime();
		
	cout << "{"<< "the number of threads = "<<  num_of_threads 
	<< "; amount of numbers = " << lastNumber 
	<< "; the time = " << end_time - start_time << "}\n";
	
	cout << "Коичество простых чисел равно = " << found << endl;
}
