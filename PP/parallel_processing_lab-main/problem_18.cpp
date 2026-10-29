// Problem_18: Divide a large dataset among MPI processes. Each process generates a local histogram, and MPI_Allreduce combines all local histograms into a global histogram. 
// Sample Input
// Number of processes: 4
// Values range from 0 to 9.
// P0: 1 2 2 3
// P1: 2 3 4 4
// P2: 1 4 5 5
// P3: 0 2 5 9
// Sample Output
// Final histogram after MPI_Allreduce:
// Value    Frequency
// 0        1
// 1        2
// 2        4
// 3        2
// 4        3
// 5        3
// 6        0
// 7        0
// 8        0
// 9        1

// Total number of elements = 20
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            cout << "Run with 4 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    int data[4][4] = {
        {1, 2, 2, 3},
        {2, 3, 4, 4},
        {1, 4, 5, 5},
        {0, 2, 5, 9}
    };

    int local[10] = {0};
    int global[10] = {0};

    // Calculate local histogram
    for (int i = 0; i < 4; i++)
        local[data[rank][i]]++;

    // Combine all local histograms
    MPI_Allreduce(local, global, 10, MPI_INT,
                  MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Final histogram after MPI_Allreduce:\n";
        cout << "Value    Frequency\n";

        for (int i = 0; i < 10; i++)
            if (global[i] > 0)
                cout << i << "        " << global[i] << endl;
    }

    MPI_Finalize();
    return 0;
}