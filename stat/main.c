#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <sysexits.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(const char* prog)
{
    printf("Print file stats\n");
    printf("Usage: %s FILE\n", prog);
}

void print_stat(const struct stat* stbuf)
{
    int avail = 1024;
    char buf[avail];
    char* cur = buf;
    int written;

    written = snprintf(cur, avail, "File type:                "); avail -= written; cur += written;
    switch (stbuf->st_mode & S_IFMT)
    {
        case S_IFDIR:
            written = snprintf(cur, avail, "Directory\n");
            break;
        case S_IFREG:
            written = snprintf(cur, avail, "Regular\n");
            break;
        default:
            snprintf(cur, avail, "Irrelevant (%o)\n", stbuf->st_mode & S_IFMT);
            return;
    }
    avail -= written; cur += written;

    written = snprintf(cur, avail, "I-node number:            %ld\n",
                       (long) stbuf->st_ino);
    avail -= written; cur += written;
    written = snprintf(cur, avail, "Mode:                     %lo (octal)\n",
                       (unsigned long) stbuf->st_mode);
    avail -= written; cur += written;
    written = snprintf(cur, avail, "Preferred I/O block size: %ld bytes\n",
                       (long) stbuf->st_blksize);
    avail -= written; cur += written;
    written = snprintf(cur, avail, "File size:                %lld bytes\n",
                        (long long) stbuf->st_size);
    avail -= written; cur += written;
    written = snprintf(cur, avail, "Blocks allocated:         %lld\n",
                       (long long) stbuf->st_blocks); avail -= written; cur += written;
    written = snprintf(cur, avail, "Link count:               %ld\n",
                       (long) stbuf->st_nlink);
    avail -= written; cur += written;
    written = snprintf(cur, avail, "Ownership:                UID=%ld   GID=%ld\n",
                       (long) stbuf->st_uid, (long) stbuf->st_gid); avail -= written; cur += written;
    written = snprintf(cur, avail, "ID of containing device:  [%lx,%lx]\n",
                       (long) major(stbuf->st_dev), (long) minor(stbuf->st_dev));
    avail -= written; cur += written;
    written = snprintf(cur, avail, "Last status change:       %s",
                       ctime(&stbuf->st_ctim.tv_sec));
    avail -= written; cur += written;
    written = snprintf(cur, avail, "Last file access:         %s",
                       ctime(&stbuf->st_atim.tv_sec));
    avail -= written; cur += written;
    written = snprintf(cur, avail, "Last file modification:   %s",
                       ctime(&stbuf->st_mtim.tv_sec));
    avail -= written; cur += written;

    printf("%s", buf);
}

void print_stat_types(const struct stat* stbuf)
{
    printf("dev_t st_dev    sizeof(dev_t): %zu\n", sizeof(dev_t));
    printf("ino_t st_ino    sizeof(ino_t): %zu [sizeof(long): %zu]\n",
            sizeof(ino_t), sizeof(long));
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }

    const char* path = argv[1];
    struct stat statbuf;

    print_stat_types(&statbuf);
    printf("\n");

    if (stat(path, &statbuf) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    print_stat(&statbuf);

    exit(EXIT_SUCCESS);
}
