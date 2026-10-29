
#include <iostream>
#include <vector>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size, n;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cout << "Enter array size: ";
        cin >> n;
    }

    // Share array size with all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> A(n), B(n);

    // Each process creates its own array
    for (int i = 0; i < n; i++)
        A[i] = rank * n + i + 1;

    // Blocking communication
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < size; i++) {
        if (i != rank) {
            MPI_Send(A.data(), n, MPI_INT, i, 0,
                     MPI_COMM_WORLD);

            MPI_Recv(B.data(), n, MPI_INT, i, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    double blocking_time = MPI_Wtime() - start;

    // Nonblocking communication
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    vector<MPI_Request> requests(2 * (size - 1));
    int count = 0;

    for (int i = 0; i < size; i++) {
        if (i != rank) {
            MPI_Irecv(B.data(), n, MPI_INT, i, 1,
                      MPI_COMM_WORLD, &requests[count++]);

            MPI_Isend(A.data(), n, MPI_INT, i, 1,
                      MPI_COMM_WORLD, &requests[count++]);
        }
    }

    MPI_Waitall(count, requests.data(), MPI_STATUSES_IGNORE);

    double nonblocking_time = MPI_Wtime() - start;

    // Print received array
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == i) {
            cout << "P" << rank << " sent: ";
            for (int j = 0; j < n; j++)
                cout << A[j] << " ";

            if (size == 2) {
                cout << "\nP" << rank << " received: ";
                for (int j = 0; j < n; j++)
                    cout << B[j] << " ";
            }
            cout << endl;
        }
    }

    if (rank == 0) {
        cout << "\nBlocking communication time = "
             << blocking_time << " seconds" << endl;

        cout << "Nonblocking communication time = "
             << nonblocking_time << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
