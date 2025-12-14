#pragma once

#include <pthread.h>
#include <unistd.h>
#include <stdexcept>
#include <string>
#include <chrono>

namespace os {

using ThreadHandle = pthread_t;
using ThreadFunc = void* (*)(void*);

ThreadHandle createThread(ThreadFunc func, void* arg);
void joinThread(ThreadHandle thread);
void detachThread(ThreadHandle thread);
void sleep(unsigned int milliseconds);
unsigned long getThreadId();

using MutexHandle = pthread_mutex_t;

MutexHandle createMutex();
void lockMutex(MutexHandle mutex);
void unlockMutex(MutexHandle mutex);
void destroyMutex(MutexHandle mutex);

using CondHandle = pthread_cond_t;

CondHandle createCondition();
void waitCondition(CondHandle cond, MutexHandle mutex);
void signalCondition(CondHandle cond);
void broadcastCondition(CondHandle cond);
void destroyCondition(CondHandle cond);

using RWLockHandle = pthread_rwlock_t;

RWLockHandle createRWLock();
void readLock(RWLockHandle lock);
void writeLock(RWLockHandle lock);
void unlockRWLock(RWLockHandle lock);
void destroyRWLock(RWLockHandle lock);

unsigned long long getCurrentTimeMillis();

}