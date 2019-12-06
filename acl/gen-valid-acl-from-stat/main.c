#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/acl.h>

void usage(char* prog_name)
{
    printf("Gen valid acl for a given file\n");
    printf("Usage: %s FILE\n", prog_name);
}

int main(int argc, char **argv)
{
    char* prog_name = argv[0];
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    char* file_path = argv[1];

    if (access(file_path, R_OK))
    {
        fprintf(stderr, "%s does not exist", file_path);
        exit(EXIT_FAILURE);
    }

    struct stat stat_buf;
    if (stat(file_path, &stat_buf) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
