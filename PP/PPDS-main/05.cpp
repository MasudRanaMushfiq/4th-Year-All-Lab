
#include <iostream>
#include <vector>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size, m, n, r, p;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> A, B, C;

    if (rank == 0) {
        cout << "Enter rows and columns of A: ";
        cin >> m >> n;

        cout << "Enter rows and columns of B: ";
        cin >> r >> p;

        if (n != r) {
            cout << "Multiplication not possible!" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        A.resize(m * n);
        B.resize(r * p);
        C.resize(m * p);

        cout << "Enter Matrix A:\n";
        for (int i = 0; i < m * n; i++)
            cin >> A[i];

        cout << "Enter Matrix B:\n";
        for (int i = 0; i < r * p; i++)
            cin >> B[i];
    }

    // Send dimensions to every process
    int dims[4];

    if (rank == 0) {
        dims[0] = m;
        dims[1] = n;
        dims[2] = r;
        dims[3] = p;
    }

    MPI_Bcast(dims, 4, MPI_INT, 0, MPI_COMM_WORLD);

    m = dims[0];
    n = dims[1];
    r = dims[2];
    p = dims[3];

    if (rank != 0)
        B.resize(r * p);

    // Send/receive matrix B
    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Send(B.data(), r * p, MPI_INT,
                     i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(B.data(), r * p, MPI_INT,
                 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Distribute rows and collect results
    for (int i = 0; i < m; i++) {
        int owner = i % size;

        if (rank == 0) {
            if (owner == 0) {
                for (int j = 0; j < p; j++) {
                    C[i * p + j] = 0;

                    for (int k = 0; k < n; k++)
                        C[i * p + j] +=
                            A[i * n + k] * B[k * p + j];
                }
            } else {
                MPI_Send(&A[i * n], n, MPI_INT,
                         owner, 1, MPI_COMM_WORLD);

                MPI_Recv(&C[i * p], p, MPI_INT,
                         owner, 2, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
            }
        } else if (rank == owner) {
            vector<int> row(n), result(p);

            MPI_Recv(row.data(), n, MPI_INT,
                     0, 1, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            for (int j = 0; j < p; j++) {
                result[j] = 0;

                for (int k = 0; k < n; k++)
                    result[j] += row[k] * B[k * p + j];
            }

            MPI_Send(result.data(), p, MPI_INT,
                     0, 2, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        cout << "\nMatrix C = A x B:\n";

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < p; j++)
                cout << C[i * p + j] << " ";
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
