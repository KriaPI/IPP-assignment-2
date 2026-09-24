#include <iostream>
#include <pthread.h>
#include <vector>

bool isPrime  (int num) {
    if(num <= 1) return false;
    for(int i=2; i<=std::sqrt(num); i++) {
        if(num % i == 0) return false;
    }
    return true;
}

struct ThreadContext {
    int id;
    int lowerBound;
    int upperBound;

    // Using char vector instead of bool vector to avoid output issues due to concurrent writes
    std::vector<char>* marked;
    std::vector<int>* seeds;
};

void* wrapper(void* arg) {
    ThreadContext* context = static_cast<ThreadContext*>(arg);
    int lowerBound = context->lowerBound;
    int upperBound = context->upperBound;

    for(int seed: *(context->seeds)) {
        // Thread lower bound should not be less than the range of the thread
        int correctedLowerBound = std::max(lowerBound, seed * seed);

        int remainder = correctedLowerBound % seed;

        int firstMultiple = correctedLowerBound - remainder + seed * (remainder != 0);

        for(int multiple = firstMultiple; multiple < upperBound; multiple += seed) {
            (*context->marked)[multiple] = 1;
        }
    }
    return nullptr;
}

int main() {

    int maxValue, it, threadCount;
    std::vector<int> seeds;

    std::cout << "Enter the maximum value: ";
    std::cin >> maxValue;

    for(it=2; it<=std::sqrt(maxValue); ++it) {
        if(isPrime(it)) {
            seeds.push_back(it);
        }
    }

    std::cout << "\nSerially computed seed prime values: ";
    for(int num: seeds) std::cout << num << " ";

    std::cout <<"\nEnter the thread count: ";
    std::cin >> threadCount;

    std::vector<pthread_t> threads(threadCount);
    std::vector<ThreadContext> contexts(threadCount);

    std::vector<char> marked(maxValue + 1, 0);

    int sqrtMaxValue = std::sqrt(maxValue);

    int parallelLowerBound = sqrtMaxValue + 1;
    int parallelUpperBound = maxValue+1;

    int chunkSize = (parallelUpperBound - parallelLowerBound) / threadCount;

    int remainingSize = (parallelUpperBound - parallelLowerBound) % threadCount;

    int currentLowerBound = parallelLowerBound;

    for(it=0; it<threadCount; ++it) {
        // Keep the chunk size variable unchanged and use a local var to assign one extra element to the first N threads dynamically
        int localChunkSize = chunkSize;

        if(it < remainingSize) ++localChunkSize;

        contexts[it].id = it;
        contexts[it].lowerBound = currentLowerBound;
        contexts[it].upperBound = currentLowerBound + localChunkSize;

        currentLowerBound += localChunkSize;

        contexts[it].marked = &marked;
        contexts[it].seeds = &seeds;
    }

    for(it=0; it<threadCount; ++it) {
        pthread_create(&threads[it], nullptr, wrapper, &contexts[it]);

    }

    for(it=0; it<threadCount; ++it) {
        pthread_join(threads[it], nullptr);
    }

    std::cout << "\n\nParallelly computed prime values: ";
    for(int num=sqrtMaxValue+1; num<=maxValue; ++num) {
        if(!marked[num]) std::cout << num << " ";
    }

    return 0;

}