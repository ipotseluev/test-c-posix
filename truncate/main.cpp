#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

void usage(const char* progname)
{
    printf("Usage: %s PATH SIZE\n", progname);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* path = argv[1];
    size_t size;
    if (access(path, F_OK))
    {
        perror("access");
        exit(EXIT_FAILURE);
    }

    {
        char* endptr;
        size = strtol(argv[2], &endptr, 10);
        if (errno || (endptr == argv[2]))
        {
            printf("Can't convert '%s' to decimal int\n", argv[2]);
            exit(EXIT_FAILURE);
        }

    }
    
    if (truncate(path, size))
    {
        perror("truncate");
        printf("errno: %d\n", errno);
        return errno;
    }
    exit(EXIT_SUCCESS);
}
