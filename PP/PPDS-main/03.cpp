
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    int data = 100;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // P0 starts the ring
    if (rank == 0)
    {
        cout << "P0 sends 100 to P" << next << endl;
        MPI_Send(&data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

        MPI_Recv(&data, 1, MPI_INT, prev, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "P0 receives 100 from P" << prev << endl;
    }
    else
    {
        MPI_Recv(&data, 1, MPI_INT, prev, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "P" << rank << " receives 100 from P"
             << prev << endl;

        cout << "P" << rank << " sends 100 to P" << next << endl;
        MPI_Send(&data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    // Wait until all communication finishes
    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    // Print in a controlled order
    for (int p = 0; p < size; p++)
    {
        if (rank == p)
        {
            // The communication messages above may appear
            // in a different order, which is normal in MPI.
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        cout << "Ring communication completed." << endl;
        cout << "Communication time = "
             << (end - start) * 1000 << " ms" << endl;
    }

    MPI_Finalize();
    return 0;
}
