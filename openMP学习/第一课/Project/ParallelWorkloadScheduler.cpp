#include<iostream>
#include<omp.h>
#include<vector>
using namespace std;

void do_work(int workload) {
    volatile long long dummy = 0;

    for (int i = 0; i < workload; i++) {
        dummy += i % 3;
    }
}


int main(){
    const int task_num = 20;
    vector<int> workload(task_num);

    //Initialize unbalanced work
    for (int i = 0; i < task_num; i++) {
        workload[i] = (i + 1) * (i + 1) * 2000000;;
    }

    double start, end;

    cout << "===== Static Scheduling =====" << endl;
    start = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < task_num; i++){
        do_work(workload[i]);
        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num()
                 << " finished task " << i
                 << " workload = " << workload[i] << endl;
        }
    }
    end = omp_get_wtime();
    cout << "Static time: " << end - start << " s\n\n";


    cout << "===== Dynamic Scheduling =====" << endl;
    start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic,1)
    for (int i = 0; i < task_num; i++) {
        do_work(workload[i]);

        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num()
                 << " finished task " << i
                 << " workload = " << workload[i] << endl;
        }
    }

    end = omp_get_wtime();
    cout << "Dynamic time: " << end - start << " s\n";

    return 0;



}