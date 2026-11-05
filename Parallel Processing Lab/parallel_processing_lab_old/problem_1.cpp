#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int a[12] = {10,20,30,40,50,60,70,80,90,100,110,120};
    int local[3];

    // Divide array
    MPI_Scatter(a, 3, MPI_INT,
                local, 3, MPI_INT,
                0, MPI_COMM_WORLD);

    // Send right boundary
    if (rank < size - 1)
    {
        MPI_Send(&local[2], 1, MPI_INT,
                 rank + 1, 0, MPI_COMM_WORLD);

        cout << "P" << rank
             << ": Right boundary sent = "
             << local[2] << endl;
    }

    // Receive left boundary
    if (rank > 0)
    {
        int x;

        MPI_Recv(&x, 1, MPI_INT,
                 rank - 1, 0,
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        cout << "P" << rank
             << ": Left boundary received = "
             << x << endl;
    }

    MPI_Finalize();
    return 0;
}