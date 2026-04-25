#include<iostream>
#include<vector>
#include<omp.h>
using namespace std;

int main(){

    const int n = 16;
    vector<int> a(n),b(n);

    //Initialize the array
    for(int i = 0; i < n; i++){
        a[i] = i + 1;
    }

    #pragma omp parallel for 
    for (int i = 0; i < n; i++){
        b[i] = a[i] * 2;

        //用来隔离每个线程的输出，实质就是上锁，简单保护变量，但是实际为串行
        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num()
                <<" handles i = " << i << endl; 
        }
    }

    cout << "Result: ";
    for (int i = 0; i < n; i++){
        cout << b[i] << " ";
    }
    cout << endl;
    return 0;
}