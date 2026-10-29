
#include <iostream>
#include <vector>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sizes[] = {64, 256, 1024, 4096, 16384, 65536, 1048576};
    int iterations = 5;

    for (int s = 0; s < 7; s++)
    {
        int bytes = sizes[s];
        int count = bytes / sizeof(char);

        vector<char> message(count, 'A');

        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();

        for (int i = 0; i < iterations; i++)
        {
            if (rank == 0)
            {
                MPI_Send(message.data(), count, MPI_CHAR, 1, 0,
                         MPI_COMM_WORLD);

                MPI_Recv(message.data(), count, MPI_CHAR, 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                MPI_Recv(message.data(), count, MPI_CHAR, 0, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                MPI_Send(message.data(), count, MPI_CHAR, 0, 0,
                         MPI_COMM_WORLD);
            }
        }

        double end = MPI_Wtime();

        if (rank == 0)
        {
            double roundTrip = (end - start) / iterations;

            cout << "Message size      : " << bytes << " bytes" << endl;
            cout << "Iterations        : " << iterations << endl;
            cout << "Total round trips : " << iterations << endl;
            cout << "Average round-trip: " << roundTrip
                 << " seconds" << endl;
            cout << "Average one-way   : " << roundTrip / 2
                 << " seconds" << endl;
            cout << "-----------------------------" << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
