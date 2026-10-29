// Problem_8: Create a two-process communication program that can cause deadlock when blocking sends are used. Modify the program to avoid deadlock using appropriate MPI communication techniques. 
// Sample Input
// Number of processes: 2
// P0 message = 100
// P1 message = 200
// Sample Output
// Unsafe version:
// P0: Waiting to send 100
// P1: Waiting to send 200
// Potential deadlock detected.
// Corrected version:
// P0 sent 100
// P1 received 100
// P1 sent 200
// P0 received 200
// Communication completed without deadlock.

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            cout << "Run with 2 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    int send_data = (rank == 0) ? 100 : 200;
    int recv_data;

    // Corrected version: avoid deadlock
    MPI_Sendrecv(&send_data, 1, MPI_INT, 1 - rank, 0,
                 &recv_data, 1, MPI_INT, 1 - rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    cout << "P" << rank << " sent " << send_data << endl;
    cout << "P" << rank << " received " << recv_data << endl;

    if (rank == 0)
        cout << "Communication completed without deadlock." << endl;

    MPI_Finalize();
    return 0;
}