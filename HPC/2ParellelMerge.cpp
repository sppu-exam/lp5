#include <iostream>
#include <ctime>
#include <omp.h>
#include <iomanip>
using namespace std;

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter array size: ";
    cin >> n;
    int* arr = new int[n];
    int* original = new int[n];

    cout << "Enter array elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        original[i] = arr[i];
    }

    clock_t start, end;
    start = clock();
    mergeSort(arr, 0, n - 1);
    end = clock();
    double seqTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Sequential Merge Sort: ";
    printArray(arr, n);

    for (int i = 0; i < n; i++) arr[i] = original[i];
    start = clock();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr, 0, n - 1);
    }
    end = clock();
    double parTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Parallel Merge Sort: ";
    printArray(arr, n);

    cout << fixed << setprecision(8);
    cout << "Sequential Time: " << seqTime << " s\n";
    cout << "Parallel Time:   " << parTime << " s\n";

    delete[] arr;
    delete[] original;
    return 0;
}