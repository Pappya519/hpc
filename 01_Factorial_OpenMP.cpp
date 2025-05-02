#include <iostream>
#include <omp.h>
using namespace std;

int serial_factorials(int number) {
    double start_time = omp_get_wtime();
    int factorial = 1;
    for (int i = 1; i <= number; i++) {
        factorial = factorial * i;
    }
    double end_time = omp_get_wtime();
    cout << "Time : " << end_time - start_time << endl;
    cout << "Your serial calculated factorial :" << factorial << endl;
    return factorial;
}

int parallel_factorials(int number) {
    double start_time = omp_get_wtime();
    int factorial = 1;
    #pragma omp parallel for reduction(*:factorial)
    for (int i = 1; i <= number; i++) {
        factorial = factorial * i;
    }
    double end_time = omp_get_wtime();
    cout << "Time : " << end_time - start_time << endl;
    cout << "Your parallel calculated factorial :" << factorial << endl;
    return factorial;
}

int main() {
    int number, serial_factorial = 0, parallel_factorial = 0;
    cout << "Enter the number :";
    cin >> number;
    serial_factorial = serial_factorials(number);
    parallel_factorial = parallel_factorials(number);
}