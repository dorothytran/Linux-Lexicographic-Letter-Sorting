/* 
Dorothy Tran 101141902 
SYSC4001 Assignment 2
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "CSORT.h"

static int set_semvalue(int sem_id);
static void del_semvalue(int sem_id);
static int semaphore_p(int sem_id);
static int semaphore_v(int sem_id);
static int sem_id;

/* Function that swaps two values */
void swap(char *x, char *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

/* Checks if the set of 3 array indexs for each process are done sorting */
int swapDone(struct shared_use_st *shared_array) {
    for(int index = 0; index < SIZE - 1; index++) 
    {
        if(shared_array->AR[index] > shared_array->AR[index + 1]) {
            return 0;
        }
    }
    return 1;
}

int main()
{
    // Initializations
    pid_t pid;
    int shmid, sem1, sem2;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_array;
    srand((unsigned int)getpid());
    char arr[SIZE];

    // Initializing two semaphores
    sem1 = semget((key_t)1235, 1, 0666 | IPC_CREAT);
    sem2 = semget((key_t)4321, 1, 0666 | IPC_CREAT);
    set_semvalue(sem1);
    set_semvalue(sem2);

    // Create shared memory
    shmid = shmget((key_t)1235, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // Make shared memory accessible in the program
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %X\n", (int)shared_memory);
    shared_array = (struct shared_use_st *)shared_memory;

    // User Input for Debug Mode
    printf("To use Debug Mode, input 1 or 0 otherwise: ");
    scanf("%d", &shared_array->debugMode);

    // Prompting the user to input 7 distinct letters and store the array in shared memory
    printf("\nPlease input 7 distinct letters by pressing the Enter key each time a letter is typed: \n");
    for(int i = 0; i < SIZE; i++) {
        scanf("%s", &arr[i]);
        shared_array->AR[i] = tolower(arr[i]); // prints out each character in lower case
    }

    // Forks three child processes that will perform the character letter swapping
    for(int i = 0; i < 3; i++)
    {
        pid = fork();
        switch(pid)
        {
            case -1:
                perror("Fork failed");
                exit(1);
            case 0: // Child process
                while(swapDone(shared_array) == 0) // while swapping is not done
                {
                    int var;
                    // Child Process 1 deals with AR[0], AR[1], and AR[2]
                    if(i == 0) 
                    {
                        if(shared_array->AR[0] > shared_array->AR[1])
                        {
                            swap(&shared_array->AR[0], &shared_array->AR[1]);
                            if(shared_array->debugMode == 1)
                            {
                                printf("Process P%d: performed swapping \n", i);
                            }
                        }
                        semaphore_p(sem1);
                        if(shared_array->AR[1] > shared_array->AR[2]) {
                            swap(&shared_array->AR[1], &shared_array->AR[2]);
                            if(shared_array->debugMode == 1)
                            {
                                printf("Process P%d: performed swapping \n", i);
                            }
                        }
                        // If no lexicographic sorting was performed
                        if(shared_array->debugMode == 1)
                        {
                            printf("Process P%d: No swapping \n", i);
                        }
                        semaphore_v(sem1);
                    }
                    // Child Process 2 deals with AR[2], AR[3], and AR[4]
                    else if(i == 1)
                    {
                        semaphore_p(sem1);
                        if(shared_array->AR[2] > shared_array->AR[3]) {
                            swap(&shared_array->AR[2], &shared_array->AR[3]);
                            if(shared_array->debugMode == 1)
                            {
                                printf("Process P%d: performed swapping \n", i);
                            }
                        }
                        semaphore_v(sem1);
                        semaphore_p(sem2);
                        if(shared_array->AR[3] > shared_array->AR[4]) {
                            swap(&shared_array->AR[3], &shared_array->AR[4]);
                            if(shared_array->debugMode == 1)
                            {
                                printf("Process P%d: performed swapping  \n", i);
                            }
                        }
                        // If no lexicographic sorting was performed
                        if (shared_array->debugMode == 1){
                            printf("Process P%d: No swapping  \n", i);
                        }
                        semaphore_v(sem2);
                    }
                    // Child Process 3 deals with AR[4], AR[5], and AR[6]
                    else if(i == 2)
                    {
                        semaphore_p(sem2);
                        if(shared_array->AR[4] > shared_array->AR[5]) {
                            swap(&shared_array->AR[4], &shared_array->AR[5]);
                            if(shared_array->debugMode == 1)
                            {
                                printf("Process P%d: performed swapping  \n", i);
                            }
                        }
                        semaphore_v(sem2);
                        if(shared_array->AR[5] > shared_array->AR[6]) {
                            swap(&shared_array->AR[5], &shared_array->AR[6]);
                            if(shared_array->debugMode == 1)
                            {
                                printf("Process P%d: performed swapping  \n", i);
                            }
                        }
                        // If no lexicographic sorting was performed
                        if(shared_array->debugMode == 1)
                        {
                            printf("Process P%d: No swapping  \n", i);
                        }
                        semaphore_v(sem2);
                    }
                }
                exit(0);
            default:
                break;        
        }
    }
    // Parent Process
    if(pid != 0)
    {
        // Wait for all child processes to finish
        for(int i = 0; i < 3; i++) 
        {
            wait(NULL);
        }

        // Print the sorted array result
        printf("\nLexicographic Sorted Array: \n");
        printf("[");
        for(int i = 0; i < SIZE; i++) {
            printf(" %c ", shared_array->AR[i]);
        }
        printf("]\n");

        // Delete the two semaphores
        del_semvalue(sem1);
        del_semvalue(sem2);

        // Detach the memory
        if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
        }
        // Delete shared memory
        if (shmctl(shmid, IPC_RMID, 0) == -1) {
            fprintf(stderr, "shmctl(IPC_RMID) failed\n");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);    
}

/* Initialize the semaphore */
static int set_semvalue(int sem_id)
{
    union semun sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1){
        return(0);
    }
    return(1);
}

/* Delete the semaphore */
static void del_semvalue(int sem_id)
{
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        fprintf(stderr, "Failed to delete semaphore\n");
    }
}

static int semaphore_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

static int semaphore_v(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}