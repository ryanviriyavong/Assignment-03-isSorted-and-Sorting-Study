#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>

// to check if vector is sorted correctly
bool isSorted(const std::vector<int>& values) {
    for (size_t i = 1; i < values.size(); i++) {
        if (values[i] < values[i - 1]) {
            return false;
        }
    }
    return true;
}

// bubble Sort
void bubbleSort(std::vector<int>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n; i++) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // stop early if it's already sorted
        if (!swapped) break;
    }
}

// selection Sort
void selectionSort(std::vector<int>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        std::swap(arr[i], arr[minIdx]);
    }
}

// insertion Sort
void insertionSort(std::vector<int>& arr) {
    size_t n = arr.size();
    for (size_t i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// quick Sort
int partition(std::vector<int>& arr, int low, int high) {
    // pick middle element as pivot so sorted arrays don't slow it down
    int mid = low + (high - low) / 2;
    std::swap(arr[mid], arr[high]);

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quickSortHelper(arr, low, p - 1);
        quickSortHelper(arr, p + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    if (!arr.empty()) {
        quickSortHelper(arr, 0, arr.size() - 1);
    }
}

// sample data depending on input type
std::vector<int> makeData(size_t size, std::string type) {
    std::vector<int> vec(size);
    if (type == "sorted") {
        for (size_t i = 0; i < size; i++) vec[i] = i;
    } else if (type == "reverse") {
        for (size_t i = 0; i < size; i++) vec[i] = size - i;
    } else { // random
        std::mt19937 rng(12345);
        std::uniform_int_distribution<int> dist(1, 50000);
        for (size_t i = 0; i < size; i++) vec[i] = dist(rng);
    }
    return vec;
}

// function to time a sort and run isSorted check
double runBench(void (*sortFunc)(std::vector<int>&), std::vector<int> arr) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = std::chrono::high_resolution_clock::now();

    // check if it worked
    if (!isSorted(arr)) {
        std::cout << "Sorting failed!\n";
    }

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    std::vector<size_t> sizes = {1000, 5000, 10000};
    std::vector<std::string> types = {"random", "sorted", "reverse"};

    std::cout << std::fixed << std::setprecision(2);

    for (const auto& type : types) {
        std::cout << "\n--- Testing " << type << " arrays ---\n";
        std::cout << "Size\t\tBubble\t\tSelection\tInsertion\tQuick\n";
        std::cout << "-------------------------------------------------------------\n";

        for (size_t sz : sizes) {
            auto d1 = makeData(sz, type);

            double tBubble = runBench(bubbleSort, d1);
            double tSelect = runBench(selectionSort, d1);
            double tInsert = runBench(insertionSort, d1);
            double tQuick  = runBench(quickSort, d1);

            std::cout << sz << "\t\t" 
                      << tBubble << " ms\t" 
                      << tSelect << " ms\t" 
                      << tInsert << " ms\t" 
                      << tQuick << " ms\n";
        }
    }

    return 0;
}