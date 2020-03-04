#include <stdio.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

// See /usr/include/x86_64-linux-gnu/bits/syscall.h on ubuntu

int main(int argc, char** argv)
{
    pid_t tid = syscall(__NR_gettid);
    printf("Tid is %u\n", tid);
    return 0;
}
