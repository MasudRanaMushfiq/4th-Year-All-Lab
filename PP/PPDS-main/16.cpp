
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int localValue = (rank + 1) * 10;
    int prefixSum;

    // Method 1: Using MPI_Scan
    MPI_Scan(&localValue, &prefixSum, 1,
             MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    cout << "P" << rank << ": Local value = "
         << localValue << ", Prefix sum = "
         << prefixSum << endl;

    int prefixResult[4];

    MPI_Gather(&prefixSum, 1, MPI_INT,
               prefixResult, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Prefix result: ";
        for (int i = 0; i < size; i++)
            cout << prefixResult[i] << " ";
        cout << endl;
    }

    // Method 2: Manual point-to-point solution
    int manualPrefix;

    if (rank == 0)
    {
        manualPrefix = localValue;

        if (size > 1)
            MPI_Send(&manualPrefix, 1, MPI_INT,
                     1, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&manualPrefix, 1, MPI_INT,
                 rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        manualPrefix += localValue;

        if (rank < size - 1)
            MPI_Send(&manualPrefix, 1, MPI_INT,
                     rank + 1, 0, MPI_COMM_WORLD);
    }

    cout << "P" << rank
         << ": Manual prefix sum = "
         << manualPrefix << endl;

    int manualResult[4];

    MPI_Gather(&manualPrefix, 1, MPI_INT,
               manualResult, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Manual prefix result: ";
        for (int i = 0; i < size; i++)
            cout << manualResult[i] << " ";
        cout << endl;
    }

    MPI_Finalize();

    return 0;
}
