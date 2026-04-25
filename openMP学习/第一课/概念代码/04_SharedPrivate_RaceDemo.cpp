#include <iostream>
#include <omp.h>
using namespace std;

int main(){
    int x = -1;


    cout << "Shared Value" << endl;
    #pragma omp parallel shared(x)          //改的是同一个x
    {
        int tid = omp_get_thread_num();   
        x = tid;
        
        #pragma omp critical
        {
            cout << "Thread " << tid << " sees x =" << x << endl;
        }

        }

        cout << "Final x = " << x << endl;



        cout << "Private Value" << endl;
        x = -1;
        #pragma omp parallel private(x)   //每个线程创建一个x
    {
        int tid = omp_get_thread_num();   //但是注意private的x不一定是外部的值，可以是任意值，此题在内部赋值，因此所有值相同
        x = tid;
        
        #pragma omp critical
        {
            cout << "Thread " << tid << " sees x =" << x << endl;
        }

        }

        cout << "Final x = " << x << endl;
        return 0;
}