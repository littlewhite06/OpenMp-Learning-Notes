#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main(){
    const int n = 10000000;
    vector<int> a(n);

    for (int i = 0; i < n; i++){
        a[i] = i % 2;
    }

    int count_critical = 0;
    int count_atomic = 0;
    int count_reduction = 0;
    int start, end;


    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (a[i] == 1) {
            #pragma omp critical
            {
                count_critical++;
            }
        }
    }
    end = omp_get_wtime();
    cout << "Critical count = " << count_critical
         << ", time = " << end - start << endl;

    start = omp_get_wtime();
    #pragma omp parallel for 
    for (int i = 0; i < n; i++){
        if (a[i] == 1){
            #pragma omp atomic   //原子操作快但是不能用于push等复杂操作，因为cpu只能处理轻量的原子指令
            count_atomic++;
        }
    }     
    end = omp_get_wtime();
    cout << "Atomic count = " << count_atomic
         << ", time = " << end - start << endl;

    // 4. reduction 版
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:count_reduction)
    for (int i = 0; i < n; i++) {
        if (a[i] == 1) {
            count_reduction++;
        }
    }
    end = omp_get_wtime();
    cout << "Reduction count = " << count_reduction
         << ", time = " << end - start << endl;

    return 0;
}