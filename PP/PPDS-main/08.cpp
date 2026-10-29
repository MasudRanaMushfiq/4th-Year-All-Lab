
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    int send_data, recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            cout << "Please run with 2 processes." << endl;

        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
        send_data = 100;
    else
        send_data = 200;

    // Unsafe version: both processes send first
    if (rank == 0)
        cout << "Unsafe version: P0 waiting to send 100" << endl;
    else
        cout << "Unsafe version: P1 waiting to send 200" << endl;

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        cout << "Potential deadlock detected." << endl;

    MPI_Barrier(MPI_COMM_WORLD);

    // Corrected version using MPI_Sendrecv
    MPI_Sendrecv(&send_data, 1, MPI_INT, 1 - rank, 0,
                 &recv_data, 1, MPI_INT, 1 - rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rank == 0) {
        cout << "Corrected version:" << endl;
        cout << "P0 sent " << send_data << endl;
        cout << "P0 received " << recv_data << endl;
    } else {
        cout << "P1 received " << recv_data << endl;
        cout << "P1 sent " << send_data << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        cout << "Communication completed without deadlock." << endl;

    MPI_Finalize();
    return 0;
}
