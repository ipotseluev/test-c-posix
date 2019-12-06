#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

void usage(char* prog_name)
{
    printf("Get realpath\n");
    printf("Usage: %s PATH\n", prog_name);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE); 
    }
    char *path = argv[1];
    char *result = realpath(path, NULL);
    if (result == NULL)
    {
        perror("Realpath");
        exit(errno);
    }

    printf("%s\n", result);
    free(result);
    exit(EXIT_SUCCESS);
}
