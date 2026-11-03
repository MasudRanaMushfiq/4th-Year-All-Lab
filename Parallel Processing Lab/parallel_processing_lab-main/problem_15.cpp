// Problem_15: Implement the trapezoidal rule for numerical integration. Each MPI process calculates a portion of the integral, and MPI_Reduce combines the partial results. 
// Sample Input
// Number of processes: 4
// Function: f(x) = x²
// Lower limit = 0
// Upper limit = 1
// Number of intervals = 1000
// Use the trapezoidal rule.
// Sample Output
// Numerical integral ≈ 0.333333
// Exact integral     = 0.333333
// Absolute error     ≈ 0.000000
// Students should report partial integrals and repeat the experiment with different numbers of intervals.

#include <iostream>
#include <cmath>
#include <mpi.h>
using namespace std;

double f(double x) {
    return x * x;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0, b = 1;
    int n = 1000;
    double h = (b - a) / n;
    double local_sum = 0, total_sum;

    for (int i = rank + 1; i < n; i += size)
        local_sum += f(a + i * h);

    double local_integral = h *
        (f(a) / 2 + local_sum + f(b) / 2);

    cout << "P" << rank << " partial integral = "
         << local_integral << endl;

    MPI_Reduce(&local_integral, &total_sum, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double exact = 1.0 / 3.0;
        cout << "Numerical integral = " << total_sum << endl;
        cout << "Exact integral     = " << exact << endl;
        cout << "Absolute error     = "
             << fabs(exact - total_sum) << endl;
    }

    MPI_Finalize();
    return 0;
}