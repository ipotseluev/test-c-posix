#include <stdlib.h>
#include <assert.h>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>


void usage(char* prog_name)
{
    printf("Read content of a dir\n");
    printf("Usage: %s DIR\n", prog_name);
}

int dir_exists(char* dir_path)
{
    int fd = open(dir_path, O_DIRECTORY);
    if (fd == -1)
    {
        return -1;
    }
    close(fd);
    return 0;
}

int posix_compliant_readdir(char* dir_path)
{
    // O_RDONLY|O_NONBLOCK|O_CLOEXEC is used by ls
    int fd = open(dir_path, O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY);
    assert(fd != -1);

    DIR* dirp = fdopendir(fd);
    if (!dirp)
    {
        perror("ERROR: opendir failed: ");
        return -1;
    }

    struct dirent* direntry = readdir(dirp);
    while (direntry != NULL)
    {
        long pos = telldir(dirp);
        assert(pos != -1);
        printf("Position: %ld Entry: %s\n", pos, direntry->d_name);
        direntry = readdir(dirp);
    }

    long pos = telldir(dirp);
    printf("End, Position: %ld\n", pos);

    closedir(dirp);

    close(fd);
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE); 
    }
    char *dir_path = argv[1];

    if (dir_exists(dir_path) == -1)
    {
        fprintf(stderr, "Directory '%s' doesn't exist\n", dir_path);
        exit(EXIT_FAILURE);
    }

    assert(!posix_compliant_readdir(dir_path));

    exit(EXIT_SUCCESS);
}
