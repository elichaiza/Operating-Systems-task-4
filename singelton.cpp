#include <pthread.h>
#include <iostream>

// Abstract Singleton Base Class
class Singleton {
protected:
    Singleton() {} // Protected constructor to prevent instantiation

public:
    // Deleting copy constructor and assignment operator
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Pure virtual function, making this class abstract
    virtual void someFunction() = 0;

    // Virtual destructor to clean up
    virtual ~Singleton() {}
};

// Derived class implementing the Singleton pattern
class DerivedSingleton : public Singleton {
private:
    static DerivedSingleton* instance; // Singleton instance
    static pthread_mutex_t mutex;      // POSIX mutex for thread safety

    // Private constructor to prevent external instantiation
    DerivedSingleton() {}

public:
    // Static method to get the Singleton instance
    static DerivedSingleton* getInstance() {
        pthread_mutex_lock(&mutex); // Locking the mutex
        if (!instance) {
            instance = new DerivedSingleton(); // Create the instance if not already created
        }
        pthread_mutex_unlock(&mutex);   // Unlocking the mutex
        return instance;
    }

    // Implementing the pure virtual function from Singleton
    void someFunction() override {
        std::cout << "Derived Singleton function." << std::endl;
    }

    // Virtual destructor to destroy the mutex
    ~DerivedSingleton() {
        pthread_mutex_destroy(&mutex);
    }

    // Public method to access the mutex
    static pthread_mutex_t* getMutex() {
        return &mutex;
    }
};

// Static member initialization
DerivedSingleton* DerivedSingleton::instance = nullptr;
pthread_mutex_t DerivedSingleton::mutex = PTHREAD_MUTEX_INITIALIZER;

// Guard (Scope Mutex) Class
class Guard {
    pthread_mutex_t* mtx;  // Pointer to the mutex

public:
    // Constructor locks the mutex
    explicit Guard(pthread_mutex_t* m) : mtx(m) {
        pthread_mutex_lock(mtx);
    }

    // Destructor unlocks the mutex
    ~Guard() {
        pthread_mutex_unlock(mtx);
    }
};

// Example function demonstrating the usage of Guard and Singleton
void someCriticalFunction() {
    Guard guard(DerivedSingleton::getMutex()); // Use public method to access mutex

    // Critical section code goes here
    std::cout << "Critical section working.\n";

    // Mutex will be unlocked when 'guard' goes out of scope
}

int main() {
    // Demonstrating the use of the derived singleton class
    DerivedSingleton* singletonInstance = DerivedSingleton::getInstance();
    singletonInstance->someFunction();

    // Call some critical function that uses the Guard class
    someCriticalFunction();

    return 0;
}
