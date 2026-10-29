// Problem_3: Implement a ring topology in which every MPI process sends a message to its next process and receives a message from its previous process. Measure the communication time for different message sizes.
// Sample Input
// Number of processes = 4
// Initial value at P0 = 100
// Communication: P0 → P1 → P2 → P3 → P0
// Sample Output
// P0 sends 100 to P1
// P1 receives 100 from P0
// P1 sends 100 to P2
// P2 receives 100 from P1
// P2 sends 100 to P3
// P3 receives 100 from P2
// P3 sends 100 to P0
// P0 receives 100 from P3
// Ring communication completed.

#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    int value;

    // P0 starts with 100
    if (rank == 0)
        value = 100;

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    // Send to next process
    if (rank == 0)
    {
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        cout << "P0 sends 100 to P1" << endl;

        MPI_Recv(&value, 1, MPI_INT, prev, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "P0 receives 100 from P3" << endl;
    }
    else
    {
        // Receive from previous process
        MPI_Recv(&value, 1, MPI_INT, prev, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "P" << rank << " receives "
             << value << " from P" << prev << endl;

        // Send to next process
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

        cout << "P" << rank << " sends "
             << value << " to P" << next << endl;
    }

    double end = MPI_Wtime();

    if (rank == 0)
    {
        cout << "\nRing communication completed." << endl;
        cout << "Communication time = "
             << end - start << " seconds" << endl;
    }

    MPI_Finalize();

    return 0;
}