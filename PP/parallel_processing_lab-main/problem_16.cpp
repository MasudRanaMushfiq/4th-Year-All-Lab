// Problem_16: Each MPI process stores a local value. Use MPI_Scan to calculate the prefix sum across all processes and compare it with a manually implemented point-to-point solution. 
// Sample Input
// Number of processes: 4
// P0 = 10
// P1 = 20
// P2 = 30
// P3 = 40
// Sample Output
// P0: Local value = 10, Prefix sum = 10
// P1: Local value = 20, Prefix sum = 30
// P2: Local value = 30, Prefix sum = 60
// P3: Local value = 40, Prefix sum = 100

// Prefix result: 10 30 60 100
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = (rank + 1) * 10;
    int prefix;

    // Method 1: MPI_Scan
    MPI_Scan(&value, &prefix, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    cout << "P" << rank << ": Local value = " << value
         << ", Prefix sum = " << prefix << endl;

    // Method 2: Manual point-to-point
    int manual = value, temp;

    if (rank > 0) {
        MPI_Recv(&temp, 1, MPI_INT, rank - 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        manual += temp;
    }

    if (rank < size - 1)
        MPI_Send(&manual, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

    if (rank == size - 1) {
        cout << "Manual prefix result = " << manual << endl;
    }

    MPI_Finalize();
    return 0;
}