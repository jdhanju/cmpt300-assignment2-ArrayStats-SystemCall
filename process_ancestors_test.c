#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define ANCESTOR_NAME_LEN 16
#define SYSCALL_PROCESS_ANCESTORS 550

struct process_info {
long pid; /* Process ID */
char name[ANCESTOR_NAME_LEN]; /* Program name of process */
long state; /* Current process state */
long uid; /* User ID of process owner */
long nvcsw; /* # voluntary context switches */
long nivcsw; /* # involuntary context switches */
long num_children; /* # children process has */
long num_siblings; /* # sibling process has */
};

int main(){

    const int SIZE = 100;
    int count = 0;
    struct process_info storage[100];

     //int result = 
     syscall(SYSCALL_PROCESS_ANCESTORS, &storage, SIZE, &count);

     for(int i = 0; i < count; i++){
        printf("pid is: %ld\n", storage[i].pid);
        printf("name is: %s\n", storage[i].name);
        printf("state is: %ld\n", storage[i].state);
        printf("nvcsw is: %ld\n", storage[i].nvcsw);
        printf("nivcsw is: %ld\n", storage[i].nivcsw);
        printf("----------------------------------------\n");

     }

    return 0;
}