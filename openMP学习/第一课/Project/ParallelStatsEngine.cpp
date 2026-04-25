#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
using namespace std;

int main(){
    const int N = 10000000;
    vector<int> a(N);

    for (int i = 0; i < N; i++){
        a[i] = i % 100;
    }

    double start;
    double end;
    long long sum = 0;
    int max_val = INT_MIN;
    int min_val = INT_MAX;
    int even_count = 0;

    //The critical version
    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++){
        #pragma omp critical 
        {
            sum += a[i];
            if (a[i] > max_val){
                max_val = a[i];
            }
            if (a[i] < min_val){
                min_val = a[i];
            }
            if (a[i] % 2 == 0){
                even_count++;
            }
        }
    }

    end = omp_get_wtime();
    cout << "The critical version: " << endl;
    cout << "Sum: " << sum << "  Even number: " << even_count << "  Maximum Value: " << max_val << "  Minimum value: " << min_val << endl;
    cout << "Time: " << end - start << endl;
    cout << endl;

    //The atomic version
    cout << "Atomic version" << endl;
    start = omp_get_wtime();
    sum = 0;
    max_val = INT_MIN;
    min_val = INT_MAX;
    even_count = 0;

    #pragma omp parallel for
    for (int i = 0; i < N; i++){
        #pragma omp atomic
            sum += a[i];

        if (a[i] % 2 == 0){
            #pragma omp atomic
            even_count++;
        }

        #pragma omp critical 
        {
            if (a[i] > max_val){
                max_val = a[i];
            }
            if (a[i] < min_val){
                min_val = a[i];
            }
        }
    }

    end = omp_get_wtime();
    cout << "The atomic version: " << endl;
    cout << "Sum: " << sum << "  Even number: " << even_count << "  Maximum Value: " << max_val << "  Minimum value: " << min_val << endl;
    cout << "Time: " << end - start << endl;
    cout << endl;

    //The reduction version
    cout << "Reduction version" << endl;
    start = omp_get_wtime();
    sum = 0;
    max_val = INT_MIN;
    min_val = INT_MAX;
    even_count = 0;

    #pragma omp parallel for reduction(+:sum,even_count) \
        reduction(max:max_val) \
        reduction(min:min_val)
    for (int i = 0; i < N; i++){
        sum += a[i];
            if (a[i] > max_val){
                max_val = a[i];
            }
            if (a[i] < min_val){
                min_val = a[i];
            }
            if (a[i] % 2 == 0){
                even_count++;
            }
    }

    end = omp_get_wtime();
    cout << "The reduction version: " << endl;
    cout << "Sum: " << sum << "  Even number: " << even_count << "  Maximum Value: " << max_val << "  Minimum value: " << min_val << endl;
    cout << "Time: " << end - start << endl;
    cout << endl;

    return 0;
}