#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void usage(const char* progname)
{
    printf("Usage: %s NEW_DIR_PATH\n", progname);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(-1);
    }
    const char* dirpath = argv[1];

    mode_t mode = 0x777; 
    int ret = mkdir(dirpath, mode);
    if (ret)
    {
        int ernum = errno;
        printf("Error: %d {%s}\n", ernum, strerror(ernum));
        exit(1);
    }

    printf("Directory was successfully created\n");
    exit(0);
}
