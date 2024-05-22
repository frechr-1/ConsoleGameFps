#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <random>

int heap_vs_stack(int iterations)
{
    const int SIZE = 100000;  // Size of the array
    std::vector<long long> timesStack, timesHeap;
    std::vector<long long> timesStackInsert, timesHeapInsert;

    for ( int run = 0; run < iterations; ++run ) {
        int stackArray[SIZE];  // Array on the stack
        auto * heapArray = new int[SIZE];  // Array on the heap

        // Populate arrays
        auto startIStack = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < SIZE; ++i ) {
            stackArray[i] = i;
        }   
        auto endIStack = std::chrono::high_resolution_clock::now();

        auto startIHeap = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < SIZE; ++i ) {
            heapArray[i] = i;
        }
        auto endIHeap = std::chrono::high_resolution_clock::now();

        volatile int temp;  // Use volatile to prevent compiler optimizations
        auto startStack = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < SIZE; ++i ) {
            temp = stackArray[i];
        }
        auto endStack = std::chrono::high_resolution_clock::now();

        auto startHeap = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < SIZE; ++i ) {
            temp = heapArray[i];
        }
        auto endHeap = std::chrono::high_resolution_clock::now();

        timesStackInsert.push_back(std::chrono::duration_cast<std::chrono::microseconds>(endIStack - startIStack).count());
        timesHeapInsert.push_back(std::chrono::duration_cast<std::chrono::microseconds>(endIHeap - startIHeap).count());
        timesStack.push_back(std::chrono::duration_cast<std::chrono::microseconds>(endStack - startStack).count());
        timesHeap.push_back(std::chrono::duration_cast<std::chrono::microseconds>(endHeap - startHeap).count());

        delete[] heapArray;  // Clean up heap memory
    }

    long long totalStack = 0, totalHeap = 0, totalInsertStack = 0, totalInsertHeap = 0;
    for ( int i = 0; i < iterations; ++i ) {
        totalInsertStack += timesStackInsert[i];
        totalInsertHeap += timesHeapInsert[i];
        totalStack += timesStack[i];
        totalHeap += timesHeap[i];
    }

    std::cout << "Average time taken for stack array init: " << (totalInsertStack / iterations) << " micros\n";
    std::cout << "Average time taken for stack array init: " << (totalInsertHeap/ iterations) << " micros\n";
    std::cout << "Average time taken for stack array access: " << (totalStack / iterations) << " micros\n";
    std::cout << "Average time taken for heap array access: " << (totalHeap / iterations) << " micros\n";

    return 0;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    int iterations = 100000; // Number of times to repeat each benchmark
    heap_vs_stack(iterations);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "\nProgram finished after " << duration << " seconds.\n";
    return 0;
}
