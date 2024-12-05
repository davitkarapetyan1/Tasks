#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;

void sumArraySequential(const vector<int>& arr, long long &result) {
    result = 0;
    for (int i = 0; i < arr.size(); ++i) {
        result += arr[i];
    }
}

void sumArrayParallel(const vector<int>& arr, long long &result, int numThreads) {
    vector<thread> threads(numThreads);
    atomic<long long> partialSum(0); 
    
    int chunkSize = arr.size() / numThreads;
    int remainder = arr.size() % numThreads;
    
    auto sumChunk = [&](int startIdx, int endIdx) {
        long long localSum = 0;
        for (int i = startIdx; i < endIdx; ++i) {
            localSum += arr[i];
        }
        partialSum += localSum;
    };
    
    int startIdx = 0;
    for (int i = 0; i < numThreads; ++i) {
        int endIdx = startIdx + chunkSize + (i < remainder ? 1 : 0); 
        threads[i] = thread(sumChunk, startIdx, endIdx);
        startIdx = endIdx;
    }

    for (auto& t : threads) {
        t.join();
    }
    
    result = partialSum.load();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <N> <M>" << endl;
        return 1;
    }

    int N = atoi(argv[1]);  
    int M = atoi(argv[2]);  

    if (N <= 1000000) {
        cerr << "N should be greater than 1,000,000." << endl;
        return 1;
    }

   
    srand(time(0));

    vector<int> arr(N);
    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 100;  
    }

    auto start = chrono::high_resolution_clock::now();
    long long sequentialSum = 0;
    sumArraySequential(arr, sequentialSum);
    auto end = chrono::high_resolution_clock::now();
    auto durationSequential = chrono::duration_cast<chrono::milliseconds>(end - start);

    start = chrono::high_resolution_clock::now();
    long long parallelSum = 0;
    sumArrayParallel(arr, parallelSum, M);
    end = chrono::high_resolution_clock::now();
    auto durationParallel = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time spent without threads: " << durationSequential.count() << " ms" << endl;
    cout << "Time spent with " << M << " threads: " << durationParallel.count() << " ms" << endl;

    return 0;
}

