// Problem_17: Each MPI process generates data intended for every other process. Implement the communication using MPI_Alltoall and analyze its communication cost as the number of processes increases. 
// Sample Input
// Number of processes: 4
// P0: 0 1 2 3
// P1: 10 11 12 13
// P2: 20 21 22 23
// P3: 30 31 32 33
// Each process sends one value to every process.
// Sample Output
// After MPI_Alltoall:
// P0 received: 0 10 20 30
// P1 received: 1 11 21 31
// P2 received: 2 12 22 32
// P3 received: 3 13 23 33
// Students should measure communication time for P = 2, 4, 8, 16 and different message sizes.

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *send_data = new int[size];
    int *recv_data = new int[size];

    for (int i = 0; i < size; i++)
        send_data[i] = rank * 10 + i;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    MPI_Alltoall(send_data, 1, MPI_INT,
                 recv_data, 1, MPI_INT, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    cout << "P" << rank << " received: ";
    for (int i = 0; i < size; i++)
        cout << recv_data[i] << " ";
    cout << endl;

    cout << "P" << rank << " communication time = "
         << end - start << " s" << endl;

    delete[] send_data;
    delete[] recv_data;

    MPI_Finalize();
    return 0;
}