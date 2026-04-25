#include <iostream>
#include <omp.h>
using namespace std;

int main(){
    const int n = 16;

    cout <<"Schedule(static)" << endl;  //同一线程处理固定区间

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++){
        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() << " processes iteration " << i << endl;
        }
    }

    cout << endl;
    cout  << "Schedule(dynamic, 1)" << endl;  //动态分配，每次分配1个迭代
    #pragma omp parallel for schedule(dynamic,1)
    for (int i = 0; i < n; i++){
        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() << " processes iteration " << i << endl;
        }
    }
    return 0;
}