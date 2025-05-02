//Dipansh Nagpal  - 20220802328
#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0)
            cout << "This program must be run with exactly 3 processes.\n";
        MPI_Finalize();
        return 0;
    }

    int ARRAY_SIZE;
    int* full_array = nullptr;
    int* sub_array = nullptr;

    if (rank == 0) {
        cout << "Enter the length (must be divisible by 2): ";
        cin >> ARRAY_SIZE;
        if (ARRAY_SIZE % 2 != 0) {
            cout << "Array size must be divisible by 2.\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        full_array = new int[ARRAY_SIZE];
        cout << "Enter elements of array:\n";
        for (int i = 0; i < ARRAY_SIZE; i++) {
            cin >> full_array[i];
        }
    }

    MPI_Bcast(&ARRAY_SIZE, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int half_size = ARRAY_SIZE / 2;
    sub_array = new int[half_size];

    MPI_Scatter(full_array, half_size, MPI_INT,
                sub_array, half_size, MPI_INT,
                0, MPI_COMM_WORLD);

    int partial_sum = 0;
    for (int i = 0; i < half_size; i++) {
        partial_sum += sub_array[i];
    }

    if (rank == 0) {
        double start_time = MPI_Wtime();
        MPI_Send(&partial_sum, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&start_time, 1, MPI_DOUBLE, 2, 1, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        MPI_Send(&partial_sum, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);
    }
    else if (rank == 2) {
        int sum1, sum2;
        double start_time, end_time;
        MPI_Recv(&sum1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&start_time, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&sum2, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int final_sum = sum1 + sum2;
        end_time = MPI_Wtime();
        cout << "Final sum: " << final_sum << endl;
        cout << "Processing time: " << end_time - start_time << " seconds" << endl;
    }

    if (full_array) delete[] full_array;
    delete[] sub_array;
    MPI_Finalize();
    return 0;
}
