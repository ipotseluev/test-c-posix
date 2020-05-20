#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 4096

void usage(const char* progname)
{
    printf("Open or create file and write SIZE bytes with value 1 starting with OFFSET\n");
    printf("Usage: %s PATH SIZE OFFSET\n", progname);
}

int main(int argc, char** argv)
{
    off_t offset;
    size_t size;

    if (argc < 3)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* path = argv[1];
    { // convert size and offset
        char* ptr;
        size = strtol(argv[2], &ptr, 10);
        if (!size || (ptr == argv[2]))
        {
            printf("Param SIZE must be a non-null integer\n");
            exit(EXIT_FAILURE);
        }
        offset = strtol(argv[3], &ptr, 10);
        if (!size || (ptr == argv[3]))
        {
            printf("Param OFFSET must be an integer\n");
            exit(EXIT_FAILURE);
        }

    }

    int fd = open(path, O_CREAT | O_RDWR);
    if (fd == -1)
    {
        perror("open");
        exit(errno);
    }

    char buf[BUFSIZE];
    for (int i = 0; i < BUFSIZE; ++i)
    {
        buf[i] = 1;
    }


    int written = 0;
    while (written < size)
    {
        size_t writesize = (size - written < BUFSIZE) ? size - written
                                                      : BUFSIZE;
        size_t ret = pwrite(fd, buf, writesize, offset + written);
        if (ret == -1)
        {
            perror("pwrite");
            printf("Error while pwrite, offset = %lu\n", offset + written);
            exit(errno);
        }

        written += ret;
    }

    printf("%lu bytes were written starting from offset %lu\n", size, offset);
    close(fd);
    exit(EXIT_SUCCESS);
}
