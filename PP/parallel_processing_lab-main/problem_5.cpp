// Problem_5: Distribute matrix rows from the root process to other processes using MPI_Send. Each process performs its portion of matrix multiplication and sends the result back to the root. 
// Sample Input
// Number of processes: 2
// Matrix A:
// 1 2
// 3 4
// Matrix B:
// 5 6
// 7 8
// Sample Output
// Matrix C = A × B
// 19 22
// 43 50

// C[0][0] = 1×5 + 2×7 = 19
// C[0][1] = 1×6 + 2×8 = 22
// C[1][0] = 3×5 + 4×7 = 43
// C[1][1] = 3×6 + 4×8 = 50

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[2][2] = {{1, 2}, {3, 4}};
    int B[2][2] = {{5, 6}, {7, 8}};
    int row[2], C[2][2];

    if (size != 2) {
        if (rank == 0)
            cout << "Run with 2 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        for (int i = 0; i < 2; i++) {
            if (i == 0) {
                row[0] = A[i][0];
                row[1] = A[i][1];
            } else {
                MPI_Send(A[i], 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
                continue;
            }

            for (int j = 0; j < 2; j++)
                C[i][j] = row[0] * B[0][j] + row[1] * B[1][j];
        }

        MPI_Recv(C[1], 2, MPI_INT, 1, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "Matrix C = A x B" << endl;
        for (int i = 0; i < 2; i++)
            cout << C[i][0] << " " << C[i][1] << endl;
    } else {
        MPI_Recv(row, 2, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int j = 0; j < 2; j++)
            C[1][j] = row[0] * B[0][j] + row[1] * B[1][j];

        MPI_Send(C[1], 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}