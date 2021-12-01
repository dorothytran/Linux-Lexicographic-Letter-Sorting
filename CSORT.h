/* Dorothy Tran 101141902 */
#define SIZE 7
#ifndef SEMUN_H
#define SEMUN_H

// Semaphore Declaration
union semun{
    int val;
    struct semid_ds* buf;
    unsigned short* array;
#if defined(__linux__)
    struct seminfo* __Buf;
#endif
    };
    #endif

// Shared Memory Declaration
struct shared_use_st {
    int debugMode;  // Debug mode integer
    char AR[SIZE];  // Intializing an array of size 7 to store in shared memory
};