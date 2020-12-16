#include <iostream>
#include <cmath>
#include <time.h>
#include <omp.h>

using namespace std;

const int array_size = 100;
const int threads = 6;

void array_generation(int *arr);
long array_sum_one(int *arr);
int array_sum_two(int *arr);
int halving_size_of_array(int half_size_of_arr);

int main()
{
	int arr[array_size];
	array_generation(arr);

	int sum = 0;
	cout << "Array: ";
	for (int i = 0; i < array_size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

	cout << "Task one: " << array_sum_one(arr) << endl;
	cout << "Task two: " << array_sum_two(arr) << endl;
}

void array_generation(int *arr) {
	srand(time(NULL));
	for (int i = 0; i < array_size; i++) {
		arr[i] = rand() % 100;
	}
}

long array_sum_one(int *arr) {
	int sum = 0;
	#pragma omp parallel for reduction (+:sum) num_threads(threads)
	for (int i = 0; i < array_size; i++)
	{
		sum += arr[i];
	}
	return sum;
}

int array_sum_two(int *arr) {
	int half_size_of_arr = array_size;
	int end_size_of_arr = array_size;

	do {
		half_size_of_arr = halving_size_of_array(half_size_of_arr);
		#pragma omp parallel for
		for (int i = 0; i < half_size_of_arr; i++) {
			int last_element_index = end_size_of_arr - i - 1;
			if (i != last_element_index) {
				arr[i] += arr[last_element_index];
				arr[last_element_index] = 0;
			}
		}
		end_size_of_arr = half_size_of_arr;
	} while (half_size_of_arr > 1);
	int sum = arr[0];
	return sum;
}

int halving_size_of_array(int half_size_of_arr) {
	if (half_size_of_arr % 2 == 0) {
		return half_size_of_arr / 2;
	}
	else {
		return (half_size_of_arr / 2) + 1;
	}
}