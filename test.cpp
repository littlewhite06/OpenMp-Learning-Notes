#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    #pragma omp parallel
    {
        #pragma omp critical
        {
            cout << "Hello from thread "
                 << omp_get_thread_num()
                 << " of "
                 << omp_get_num_threads()
                 << endl;
        }
    }

    return 0;
}