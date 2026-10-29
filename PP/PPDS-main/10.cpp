
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int items[8] = {10, 20, 30, 40, 50, 60, 70, 80};

    if (rank == 0) {
        // Producer
        for (int i = 0; i < 8; i++) {
            int consumer = (i % 2) + 1;

            cout << "P0 produced: " << items[i] << endl;

            MPI_Send(&items[i], 1, MPI_INT,
                     consumer, 0, MPI_COMM_WORLD);
        }

        int done;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&done, 1, MPI_INT, i,
                     1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        cout << "All 8 items processed successfully." << endl;
    }
    else {
        // Consumers
        int item;

        for (int i = 0; i < 8; i++) {
            // Each consumer receives its assigned items
            if ((i % 2) + 1 == rank) {
                MPI_Recv(&item, 1, MPI_INT, 0,
                         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                cout << "P" << rank
                     << " consumed: " << item << endl;
            }
        }

        int done = 1;
        MPI_Send(&done, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
