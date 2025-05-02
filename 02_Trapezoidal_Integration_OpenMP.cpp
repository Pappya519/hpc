#include <iostream>
#include <omp.h>
using namespace std;

double f(double x) {
    return x * x;
}

int main() {
    double a, b;
    int n, num_threads;
    cout << "Enter lower limit (a): ";
    cin >> a;
    cout << "Enter upper limit (b): ";
    cin >> b;
    cout << "Enter number of trapezoids (n): ";
    cin >> n;
    cout << "Enter number of threads: ";
    cin >> num_threads;

    double h = (b - a) / n;
    double integral = 0.0;
    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(num_threads)
    {
        double local_sum = 0.0;
        #pragma omp for
        for (int i = 1; i < n; i++) {
            double x = a + i * h;
            local_sum += 2.0 * f(x);
        }
        #pragma omp critical
        {
            integral += local_sum;
        }
    }

    integral += f(a) + f(b);
    integral *= (h / 2.0);
    double end_time = omp_get_wtime();

    cout << "Approximate value of the integral: " << integral << endl;
    cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    return 0;
}