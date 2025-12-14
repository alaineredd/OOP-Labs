#include "os.h"

namespace os {
    
ThreadHandle createThread(ThreadFunc func, void* arg) {
    pthread_t thread;
    if (pthread_create(&thread, NULL, func, arg) != 0) {
        throw std::runtime_error("Failed to create thread");
    }
    return thread;
}

void joinThread(ThreadHandle thread) {
    pthread_join(thread, NULL);
}

void detachThread(ThreadHandle thread) {
    pthread_detach(thread);
}

void sleep(unsigned int milliseconds) {
    usleep(milliseconds * 1000);
}

unsigned long getThreadId() {
    return (unsigned long)pthread_self();
}

MutexHandle createMutex() {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    return mutex;
}

void lockMutex(MutexHandle mutex) {
    pthread_mutex_lock(&mutex);
}

void unlockMutex(MutexHandle mutex) {
    pthread_mutex_unlock(&mutex);
}

void destroyMutex(MutexHandle mutex) {
    pthread_mutex_destroy(&mutex);
}

CondHandle createCondition() {
    pthread_cond_t cond;
    pthread_cond_init(&cond, NULL);
    return cond;
}

void waitCondition(CondHandle cond, MutexHandle mutex) {
    pthread_cond_wait(&cond, &mutex);
}

void signalCondition(CondHandle cond) {
    pthread_cond_signal(&cond);
}

void broadcastCondition(CondHandle cond) {
    pthread_cond_broadcast(&cond);
}

void destroyCondition(CondHandle cond) {
    pthread_cond_destroy(&cond);
}

RWLockHandle createRWLock() {
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, NULL);
    return lock;
}

void readLock(RWLockHandle lock) {
    pthread_rwlock_rdlock(&lock);
}

void writeLock(RWLockHandle lock) {
    pthread_rwlock_wrlock(&lock);
}

void unlockRWLock(RWLockHandle lock) {
    pthread_rwlock_unlock(&lock);
}

void destroyRWLock(RWLockHandle lock) {
    pthread_rwlock_destroy(&lock);
}

unsigned long long getCurrentTimeMillis() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
}