#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main(){
    const int n = 1000000;
    vector<int> a(n,1);

    long long sum = 0;

    #pragma omp parallel for 
    for (int i = 0; i < n; i++){
        #pragma omp critical 
        {
            sum += a[i];
        }
    }

    cout << "Critical sum = " << sum << endl;

    return 0;
}

