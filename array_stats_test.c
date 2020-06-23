#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYSCALL_ARRAY_STATS 549

struct array_stats_s
{
    long min;
    long max;
    long sum;
};

int main()
{

    struct array_stats_s stats;
    const long SIZE = 6;
    long array[6] = {1, 2, 3, 4, 5, 6};
    //long x = 5;
    //long* test = &x + 70000000;

    int result = syscall(SYSCALL_ARRAY_STATS, &stats, &array, SIZE);

    printf("------Test with expected normal data---------\n");

    printf("Result is %d\n", result);

    if (result == 0)
    {
        printf("The min is: %ld\n", stats.min);
        printf("The max is: %ld\n", stats.max);
        printf("The sum is: %ld\n", stats.sum);
    }
    printf("\n\n\n");

    printf("--------------Test with size < 0--------------\n");
    int result2 = syscall(SYSCALL_ARRAY_STATS, &stats, &array, -1);
    printf("Result is %d\n", result2);
    printf("\n\n\n");

    printf("--------------Test with size = 0--------------\n");
    int result3 = syscall(SYSCALL_ARRAY_STATS, &stats, &array, 0);
    printf("Result is %d\n", result3);
    printf("\n\n\n");

    printf("--------------Test with array pointer = null--------------\n");
    int result4 = syscall(SYSCALL_ARRAY_STATS, &stats, NULL, SIZE);
    printf("Result is %d\n", result4);
    printf("\n\n\n");

    printf("--------------Test with stats pointer = null--------------\n");
    int result5 = syscall(SYSCALL_ARRAY_STATS, NULL, &array, SIZE);
    printf("Result is %d\n", result5);
    printf("\n\n\n");

    printf("--------------Test with array size = 10000--------------\n");
    int result6 = syscall(SYSCALL_ARRAY_STATS, &stats, &array, 10000);
    printf("Result is %d\n", result6);
    printf("\n\n\n");

    printf("--------------Test by adding 10000 to the array pointer--------------\n");
    int result7 = syscall(SYSCALL_ARRAY_STATS, &stats, &array + 10000, 10000);
    printf("Result is %d\n", result7);
    printf("\n\n\n");

    return 0;
}