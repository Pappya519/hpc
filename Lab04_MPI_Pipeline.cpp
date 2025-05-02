//Dipansh Nagpal  - 20220802328
#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int arr_size = 5;
    float full_array[arr_size] = { 1.0,2.0,3.0,4.0,5.0 }, result_array[arr_size];

    if (size != 4) {
        if (rank == 0) {
            cout << "There should be 4 ranks";
            return 0;
        }
    }
    else {
        if (rank == 0) {
            for (int i = 0; i < arr_size;i++) {
                float partial_step = full_array[i] - 4;
                MPI_Send(&partial_step, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
            }
        }
        else if (rank == 1) {
            for (int i = 0; i < arr_size;i++) {
                float partial_step;
                MPI_Recv(&partial_step, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                partial_step /= 5;
                MPI_Send(&partial_step, 1, MPI_FLOAT, 2, 0, MPI_COMM_WORLD);
            }
        }
        else if (rank == 2) {
            for (int i = 0; i < arr_size;i++) {
                float partial_step;
                MPI_Recv(&partial_step, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                partial_step += 3;
                MPI_Send(&partial_step, 1, MPI_FLOAT, 3, 0, MPI_COMM_WORLD);
            }
        }
        else if (rank == 3) {
            cout << "Final result :\n";
            for (int i = 0; i < arr_size;i++) {
                float partial_step;
                MPI_Recv(&partial_step, 1, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                result_array[i] = partial_step * 7;
                cout << result_array[i] << " ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
