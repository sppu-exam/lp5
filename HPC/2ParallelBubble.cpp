#include <iostream>
#include <ctime>
#include <omp.h>
#include <iomanip>
using namespace std;

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallelized version using odd-even transposition sort
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = (i % 2); j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
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
    bubbleSort(arr, n);
    end = clock();
    double seqTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Sequential Bubble Sort: ";
    printArray(arr, n);

    for (int i = 0; i < n; i++) arr[i] = original[i];
    start = clock();
    parallelBubbleSort(arr, n);
    end = clock();
    double parTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Parallel Bubble Sort: ";
    printArray(arr, n);

    cout << fixed << setprecision(8);
    cout << "Sequential Time: " << seqTime << " s\n";
    cout << "Parallel Time:   " << parTime << " s\n";

    delete[] arr;
    delete[] original;
    return 0;
}