#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    printf("Time is ticking\n");

    time_t begin_time = time(NULL);
    struct timespec begin_clock;
    clock_gettime(CLOCK_REALTIME, &begin_clock);;

    printf("Type enter to stop\n");
    getchar();

    

    time_t end_time = time(NULL);
    struct timespec end_clock;
    clock_gettime(CLOCK_REALTIME, &end_clock);

    printf("time: %lu seconds passed\n", end_time - begin_time);
    printf("clock_gettime: %lu seconds passed\n", end_clock.tv_sec - begin_clock.tv_sec);
    printf("clock_gettime: %lu nanoseconds passed\n", end_clock.tv_nsec - begin_clock.tv_nsec);
    exit(EXIT_SUCCESS);
}
