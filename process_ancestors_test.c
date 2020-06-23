#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define ANCESTOR_NAME_LEN 16
#define SYSCALL_PROCESS_ANCESTORS 550

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition)                                                                          \
    do                                                                                            \
    {                                                                                             \
        if (!(condition))                                                                         \
        {                                                                                         \
            printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
            exit(1);                                                                              \
        }                                                                                         \
    } while (0)

struct process_info
{
    long pid;                     /* Process ID */
    char name[ANCESTOR_NAME_LEN]; /* Program name of process */
    long state;                   /* Current process state */
    long uid;                     /* User ID of process owner */
    long nvcsw;                   /* # voluntary context switches */
    long nivcsw;                  /* # involuntary context switches */
    long num_children;            /* # children process has */
    long num_siblings;            /* # sibling process has */
};

int main()
{

    const long SIZE = 100;
    int count = 0;
    struct process_info storage[100];

    //int result =
    syscall(SYSCALL_PROCESS_ANCESTORS, &storage, SIZE, &count);
    printf("\n");
    printf("PID\tState\tName\tUID\tnvcsw\tnivcsw\tchild\tsiblings\n");

    for (int i = 0; i < count; i++)
    {
        printf("%ld\t", storage[i].pid);
        printf("%ld\t", storage[i].state);
        printf("%.5s\t", storage[i].name);
        printf("%ld\t", storage[i].uid);
        printf("%ld\t", storage[i].nvcsw);
        printf("%ld\t", storage[i].nivcsw);
        printf("%ld\t", storage[i].num_children);
        printf("%ld\t", storage[i].num_siblings);
        printf("\n");
    }

    printf("\n---------------------Now Checking edge cases-----------------------\n");
    CHECK(syscall(SYSCALL_PROCESS_ANCESTORS, &storage, SIZE, &count) == 0);
    CHECK(syscall(SYSCALL_PROCESS_ANCESTORS, &storage, 0, &count) == -1);
    CHECK(syscall(SYSCALL_PROCESS_ANCESTORS, &storage, (long)-1, &count) == -1);
    CHECK(syscall(SYSCALL_PROCESS_ANCESTORS, &storage, (long)-12345, &count) == -1);
    CHECK(syscall(SYSCALL_PROCESS_ANCESTORS, &storage, SIZE, NULL) == -1);
    CHECK(syscall(SYSCALL_PROCESS_ANCESTORS, &storage, SIZE, &count + 10000) == -1);


    printf("CONGRATS ALL TESTS MADE BY ME PASSED!! WOOOOO\n");
    

    return 0;
}