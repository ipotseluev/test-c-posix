#include <stdio.h>

int usage(char* prog, int retcode)
{
    printf("Renames files or directories\n");
    printf("Usage: %s OLDPATH NEWPATH\n", prog);
    return retcode;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        return usage(argv[0], -1);
    }

    const char* oldpath = argv[1];
    const char* newpath = argv[2];

    int ret = rename(oldpath, newpath);
    if (ret)
    {
        perror("rename");
    }
    return 0;
}
