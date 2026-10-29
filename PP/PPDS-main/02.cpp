
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    int row[4];
    int sum = 0;
    int total = 0;

    if (rank == 0)
    {
        // Root process calculates its own row
        for (int i = 0; i < 4; i++)
        {
            row[i] = matrix[0][i];
        }

        // Root sends one row to each worker
        for (int p = 1; p < size; p++)
        {
            MPI_Send(matrix[p], 4, MPI_INT, p, 0,
                     MPI_COMM_WORLD);
        }
    }
    else
    {
        // Workers receive their rows
        MPI_Recv(row, 4, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Every process calculates its row sum
    for (int i = 0; i < 4; i++)
    {
        if (rank == 0)
            sum += matrix[0][i];
        else
            sum += row[i];
    }

    // Print each process's row and sum
    cout << "P" << rank << ": Row = ";

    for (int i = 0; i < 4; i++)
    {
        if (rank == 0)
            cout << matrix[0][i] << " ";
        else
            cout << row[i] << " ";
    }

    cout << "  Sum = " << sum << endl;

    // Workers send their sums to root
    if (rank != 0)
    {
        MPI_Send(&sum, 1, MPI_INT, 0, 1,
                 MPI_COMM_WORLD);
    }
    else
    {
        total = sum;

        // Root receives worker sums
        for (int p = 1; p < size; p++)
        {
            int workerSum;
            MPI_Recv(&workerSum, 1, MPI_INT, p, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            total += workerSum;
        }

        cout << "Total matrix sum = " << total << endl;
    }

    MPI_Finalize();
    return 0;
}

// To run this program, you can use the following command in the terminal:
// nano 02.cpp (CTRL+O, ENTER, CTRL+X)
// mpic++ 02.cpp -o 02
// mpirun -np 4 ./02
