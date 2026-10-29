
#include <iostream>
#include <mpi.h>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Number of values sent to each process
    int count = 1;
    int repeat = 1000;

    if (argc > 1)
        count = atoi(argv[1]);

    if (argc > 2)
        repeat = atoi(argv[2]);

    int *sendData = new int[size * count];
    int *recvData = new int[size * count];

    // Generate data for every process
    for (int dest = 0; dest < size; dest++)
    {
        for (int j = 0; j < count; j++)
        {
            sendData[dest * count + j] =
                rank * 10 + dest + j * size * 10;
        }
    }

    // Measure communication time
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < repeat; i++)
    {
        MPI_Alltoall(sendData, count, MPI_INT,
                     recvData, count, MPI_INT,
                     MPI_COMM_WORLD);
    }

    double end = MPI_Wtime();
    double localTime = (end - start) / repeat;
    double maxTime;

    // Find the maximum time among processes
    MPI_Reduce(&localTime, &maxTime, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    // Print received data
    cout << "P" << rank << " received: ";

    for (int source = 0; source < size; source++)
    {
        for (int j = 0; j < count; j++)
        {
            cout << recvData[source * count + j] << " ";
        }
    }

    cout << endl;

    if (rank == 0)
    {
        cout << "Processes = " << size << endl;
        cout << "Values per process = " << count << endl;
        cout << "Average Alltoall time = "
             << maxTime << " seconds" << endl;
    }

    delete[] sendData;
    delete[] recvData;

    MPI_Finalize();

    return 0;
}
