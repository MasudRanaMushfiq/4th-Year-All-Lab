// Problem_4: Implement a ping-pong program between two MPI processes using MPI_Send and MPI_Recv. Test message sizes from a few bytes to several megabytes and analyze communication latency and bandwidth. 
// Sample Input
// Number of processes: 2
// Number of iterations = 5
// Message size = 1024 bytes
// Sample Output
// Message size      	: 1024 bytes
// Iterations         	: 5
// Total round trips	: 5
// Average round-trip	: 0.000018 seconds
// Average one-way    	: 0.000009 seconds
// Repeat the experiment for 64 bytes, 256 bytes, 1 KB, 4 KB, 16 KB, 64 KB, and 1 MB.

#include <iostream>
#include <vector>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, iterations = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            cout << "Run with 2 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    int sizes[] = {64, 256, 1024, 4096, 16384, 65536, 1048576};

    for (int s : sizes) {
        vector<char> data(s, 'A');
        char dummy = 'A';

        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();

        for (int i = 0; i < iterations; i++) {
            if (rank == 0) {
                MPI_Send(data.data(), s, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(data.data(), s, MPI_CHAR, 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(data.data(), s, MPI_CHAR, 0, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(data.data(), s, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }

        double end = MPI_Wtime();

        if (rank == 0) {
            double round_trip = (end - start) / iterations;

            cout << "Message size: " << s << " bytes" << endl;
            cout << "Iterations: " << iterations << endl;
            cout << "Average round-trip: " << round_trip
                 << " seconds" << endl;
            cout << "Average one-way: " << round_trip / 2
                 << " seconds" << endl;
            cout << "----------------------" << endl;
        }
    }

    MPI_Finalize();
    return 0;
}