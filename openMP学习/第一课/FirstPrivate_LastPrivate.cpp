#include <iostream>
#include <omp.h>
using namespace std;

int main(){
    int x = 100;

    cout << "Before parallel, x = " << x << endl;

    #pragma omp parallel for firstprivate(x) lastprivate(x) //firstprivate 保证每个线程有自己的x，并且初始值从外部读入
    for (int i = 0; i < 8; i++){                            //lastprivate 把最后一次迭代的x送回循环外部
        x = x + i;

        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num()
            << " handle i = " << i << ", private x = " << x << endl; 
        }
    }

    cout << "After parallel, x = " << x << endl;

    return 0;
}