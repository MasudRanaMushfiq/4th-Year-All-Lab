// Problem_10: Design a producer–consumer system where one or more MPI processes generate data and another process receives and processes the data using point-to-point communication. 
// Sample Input
// Number of processes: 3
// P0 = Producer
// P1 = Consumer
// P2 = Consumer
// Number of items = 8
// Items: 10 20 30 40 50 60 70 80
// Sample Output
// P0 produced: 10
// P1 consumed: 10
// P0 produced: 20
// P2 consumed: 20
// P0 produced: 30
// P1 consumed: 30
// P0 produced: 40
// P2 consumed: 40
// ...
// All 8 items processed successfully.

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0)
            cout << "Run with 3 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    int items[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int data, consumer;

    if (rank == 0) {  // Producer
        for (int i = 0; i < 8; i++) {
            consumer = (i % 2 == 0) ? 1 : 2;

            cout << "P0 produced: " << items[i] << endl;
            MPI_Send(&items[i], 1, MPI_INT, consumer, 0,
                     MPI_COMM_WORLD);
        }

        data = -1;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    } else {  // Consumers
        while (true) {
            MPI_Recv(&data, 1, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (data == -1)
                break;

            cout << "P" << rank << " consumed: "
                 << data << endl;
        }
    }

    if (rank == 0)
        cout << "All 8 items processed successfully." << endl;

    MPI_Finalize();
    return 0;
}