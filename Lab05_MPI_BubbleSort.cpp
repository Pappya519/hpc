//Dipansh Nagpal  - 20220802328
#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void printArray(const vector<int>& arr, int rank) {
    cout << "Rank " << rank << " has: ";
    for (int num : arr)
        cout << num << " ";
    cout << endl;
}

int main(int argc, char** argv) {
    int rank, size;
    const int total_size = 8;
    int full_array[total_size] = { 23, 12, 4, 56, 9, 1, 44, 17 };
    vector<int> local_array;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = total_size / size;
    local_array.resize(chunk_size);
    MPI_Scatter(full_array, chunk_size, MPI_INT, local_array.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    sort(local_array.begin(), local_array.end());

    for (int phase = 0; phase < size; ++phase) {
        if ((phase % 2 == 0 && rank % 2 == 0) || (phase % 2 == 1 && rank % 2 == 1)) {
            if (rank + 1 < size) {
                vector<int> neighbor_array(chunk_size);
                MPI_Sendrecv(local_array.data(), chunk_size, MPI_INT, rank + 1, 0,
                             neighbor_array.data(), chunk_size, MPI_INT, rank + 1, 0,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                vector<int> merged(chunk_size * 2);
                merge(local_array.begin(), local_array.end(), neighbor_array.begin(), neighbor_array.end(), merged.begin());
                copy(merged.begin(), merged.begin() + chunk_size, local_array.begin());
            }
        }
        else if (rank - 1 >= 0) {
            vector<int> neighbor_array(chunk_size);
            MPI_Sendrecv(local_array.data(), chunk_size, MPI_INT, rank - 1, 0,
                         neighbor_array.data(), chunk_size, MPI_INT, rank - 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<int> merged(chunk_size * 2);
            merge(neighbor_array.begin(), neighbor_array.end(), local_array.begin(), local_array.end(), merged.begin());
            copy(merged.begin() + chunk_size, merged.end(), local_array.begin());
        }
    }

    MPI_Gather(local_array.data(), chunk_size, MPI_INT, full_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "\nFinal Sorted Array:\n";
        for (int i = 0; i < total_size; ++i)
            cout << full_array[i] << " ";
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
