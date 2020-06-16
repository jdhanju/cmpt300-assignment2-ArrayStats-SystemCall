#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYSCALL_ARRAY_STATS 549

struct array_stats_s {
	long min;
	long max;
	long sum;
};

int main(){

    struct array_stats_s stats;
    const long SIZE = 6; 
    long array[6] = {1, 2, 3, 4, 5, 6};
    //long x = 5;   
    //long* test = &x + 70000000;

    int result = syscall(SYSCALL_ARRAY_STATS, &stats, &array, SIZE );

    printf("Result is %d\n", result);

    if(result == 0){
        printf("The min is: %ld\n", stats.min);
        printf("The max is: %ld\n", stats.max);
        printf("The sum is: %ld\n", stats.sum);
    }

    for(int i = 0; i < 5; i++){
        //printf("data i %ld\n", array[i]);
    }

    return 0;
}