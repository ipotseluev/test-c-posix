#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int usage(char* prog)
{
    printf("Renames files or directories\n");
    printf("Usage: %s OLDPATH NEWPATH\n", prog);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* oldpath = argv[1];
    const char* newpath = argv[2];

    int ret = rename(oldpath, newpath);
    if (ret)
    {
        perror("rename");
        exit(errno);
    }
    exit(EXIT_SUCCESS);
}
