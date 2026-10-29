
#include <iostream>
#include <mpi.h>
#include <cmath>
#include <iomanip>
using namespace std;

double f(double x)
{
    return x * x;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0, b = 1.0;
    int n = 1000;

    // Divide intervals among processes
    int count = n / size;
    int start = rank * count;

    if (rank == size - 1)
        count = n - start;

    double h = (b - a) / n;
    double localIntegral = 0.0;

    // Calculate partial integral
    for (int i = start; i < start + count; i++)
    {
        double x1 = a + i * h;
        double x2 = a + (i + 1) * h;

        localIntegral += (f(x1) + f(x2)) * h / 2.0;
    }

    cout << "P" << rank << " partial integral = "
         << fixed << setprecision(9)
         << localIntegral << endl;

    double totalIntegral;

    // Combine partial integrals at root
    MPI_Reduce(&localIntegral, &totalIntegral, 1,
               MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        double exact = 1.0 / 3.0;
        double error = fabs(totalIntegral - exact);

        cout << fixed << setprecision(9);
        cout << "\nNumerical integral = " << totalIntegral << endl;
        cout << "Exact integral     = " << exact << endl;
        cout << "Absolute error     = " << error << endl;
    }

    MPI_Finalize();

    return 0;
}
