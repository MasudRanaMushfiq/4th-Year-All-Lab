

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int iterations, threshold, blockSize;

    // Root process reads the parameters
    if (rank == 0)
    {
        iterations = 1000;
        threshold = 50;
        blockSize = 256;
    }

    // Broadcast parameters to all processes
    MPI_Bcast(&iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&threshold, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&blockSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Every process prints the received parameters
    cout << "P" << rank << " received: "
         << "Iterations = " << iterations
         << ", Threshold = " << threshold
         << ", Block size = " << blockSize
         << endl;

    MPI_Finalize();

    return 0;
}
