#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

void usage(const char* progname)
{
    printf("Update times of file");
    printf("Usage: %s FILEPATH\n", progname);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* path = argv[1];

    // times[0] - atime; times[1] - mtime
    struct timespec times[2];
    // 14.07.1987
    times[0].tv_sec = 553241166;
    times[0].tv_nsec = 161616161;

    // current time
    times[1].tv_nsec = UTIME_NOW;

    // current time to 

    // do not touch
    times[1].tv_nsec = UTIME_OMIT;

    // dirfd is AT_FDCWD for relative path
    // dirdf is ignored for absolute path
     int ret = utimensat(AT_FDCWD, path, times, 0);

    // set current time to both
    // int ret = utimensat(AT_FDCWD, path, NULL, 0);
    if (ret)
    {
        perror("utimensat:");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
