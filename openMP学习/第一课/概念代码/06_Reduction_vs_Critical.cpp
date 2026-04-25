#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main(){
    const int n = 10000000;
    vector<int> a(n);

    for (int i = 0; i < n; i++){
        a[i] = i;
    }

    long long sum_critical = 0;
    long long sum_reduction = 0;

    double start,end;

    cout << "The critical version" << endl;
    start = omp_get_wtime();

    #pragma omp parallel for 
    for(int i = 0; i < n; i++){
        #pragma omp critical              //每次都要上锁
        {
            sum_critical += a[i];
        }
    }

    end = omp_get_wtime();
    cout << "Critical sum = " << sum_critical << ", time = " << end - start << " second" << endl;

    cout << "The reduction version" << endl;
    start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum_reduction)
    for (int i = 0; i < n; i++){
        sum_reduction += a[i];
    }

    end = omp_get_wtime();
    cout << "Reduction sum = " << sum_reduction << ", time = " << end - start << " second" << endl;
    return 0;

}