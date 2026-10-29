
#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int data[4][4] = {
        {1, 2, 2, 3},
        {2, 3, 4, 4},
        {1, 4, 5, 5},
        {0, 2, 5, 9}
    };

    int local[4][10] = {0};
    int histogram[10] = {0};

    #pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();

        for (int i = 0; i < 4; i++) {
            int value = data[id][i];
            local[id][value]++;
        }
    }

    // Combine local histograms
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            histogram[j] += local[i][j];
        }
    }

    cout << "Final histogram using OpenMP:\n";
    cout << "Value    Frequency\n";

    for (int i = 0; i < 10; i++) {
        cout << i << "        " << histogram[i] << endl;
    }

    cout << "Total number of elements = 16" << endl;

    return 0;
}
