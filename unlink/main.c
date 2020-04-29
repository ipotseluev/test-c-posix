#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage(const char* progname)
{
    printf("Unlinks file\n");
    printf("Usage: %s PATH\n", progname);

}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* path = argv[1];

    if (access(path, F_OK))
    {
        perror("access");
        exit(errno);
    }

    struct stat statBeforeUnlink, statAfterUnlink;

    int fd1 = open(path, O_RDWR);
    int fd2 = open(path, O_RDWR);

    if (fstat(fd1, &statBeforeUnlink))
    {
        perror("stat");
        exit(errno);
    }

    int ret = unlink(path);
    if (ret)
    {
        perror("unlink");
        exit(errno);
    }

    if (fstat(fd1, &statAfterUnlink))
    {
        perror("stat after unlink");
        exit(errno);
    }

    if ((statBeforeUnlink.st_size != statAfterUnlink.st_size)
        || (statBeforeUnlink.st_blocks != statAfterUnlink.st_blocks))

    {
        printf("Stats size or blocks are not matching before and after unlink!\n");
    }

    printf("unlink(%s) called. 2 descriptors are opened\n", path);
    printf("Press Enter to close 1/2 descriptor\n");
    int enter = 0;
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
    close(fd1);

    enter = 0;
    printf("Press Enter to close 2/2 descriptor\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
    close(fd2);

    exit(EXIT_SUCCESS);
}
