#pragma once
#include <atomic>


/// @brief An implementation of a Test-and-Test-and-Set (TTAS or TATAS) lock. 
class TATASLock {
    std::atomic_bool isLocked {false};

    public:
        TATASLock() noexcept = default;
        ~TATASLock() = default;

        // Make sure that the lock cannot be copied.
        TATASLock(const TATASLock&) = delete;
        TATASLock& operator=(const TATASLock&) = delete;

        /// @brief Aquire the lock.
        void lock() {
            while (true) {
                while (isLocked) {}
                if (isLocked.exchange(true) == false) {
                    // We've acquired the lock and can stop blocking!
                    return;
                }
            }
        }

        /// @brief Release the lock.
        void unlock() {
            isLocked = false;
        }
};


/// @brief An implementation of a CLH lock. 
class CLHLock {
    // For the sake of avoiding false sharing, we set an alignment requirement for 64 bytes. 
    struct alignas(64) QueueNode {
        std::atomic_bool locked{false};
    };

    std::atomic<QueueNode*> tail;
    
    static thread_local QueueNode* myNode;
    static thread_local QueueNode* myPrevious;

public:
    CLHLock() {
        tail.store(new QueueNode{false}, std::memory_order_relaxed);
    }

    void lock() {
        // Yes, this will leak, but we don't care. 
        if (!myNode) myNode = new QueueNode();
        
        myNode->locked.store(true, std::memory_order_relaxed);
        
        QueueNode* predecessor = tail.exchange(myNode, std::memory_order_acq_rel);
        myPrevious = predecessor;

        while (predecessor->locked.load(std::memory_order_acquire)) {}
    }

    void unlock() {
        myNode->locked.store(false, std::memory_order_release);
        myNode = myPrevious; 
    }
};

thread_local CLHLock::QueueNode* CLHLock::myNode = nullptr;
thread_local CLHLock::QueueNode* CLHLock::myPrevious = nullptr;