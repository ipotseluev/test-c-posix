#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void usage(char* prog_name) {
    printf("Opens file\n");
    printf("Usage: %s DIR\n", prog_name);
}

void print_open_flags(int flags)
{
    char* buf_cur = malloc(1024);
    assert(buf_cur);

    const char* buf_begin = buf_cur;
    const char* buf_end = &buf_cur[1024];

    int append_flag_to_str(char **buf_cur,
                           const char *buf_end,
                           int *bitfield,
                           const char *flag_str,
                           int flag)
    {
        int written;
        // O_TMPFILE consists of two bits
        if ((*bitfield & flag) == flag)
        {
            written = snprintf(*buf_cur, buf_end - *buf_cur, "%s ", flag_str);
            if (written < 0)
            {
                fprintf(stderr, "Insufficient buffer size while writing flag %s\n", flag_str);
                return -1;
            }
            *buf_cur += written;
            *bitfield &= ~flag;
            return 0;
        }
        return -1;
    }

    if (append_flag_to_str(&buf_cur, buf_end, &flags, "O_WRONLY", O_WRONLY) < 0)
    {
        // O_RDONLY is a very special flag, it is 0 (aka 'not O_WRONLY')
        int written = snprintf(buf_cur, buf_end - buf_cur, "%s ", "O_RDONLY");
        assert(written != -1);
        buf_cur += written; 
    }

    append_flag_to_str(&buf_cur, buf_end, &flags, "O_RDWR",        O_RDWR);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_ACCMODE",     O_ACCMODE);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_CREAT",       O_CREAT);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_EXCL",        O_EXCL);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_NOCTTY",      O_NOCTTY);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_TRUNC",       O_TRUNC);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_APPEND",      O_APPEND);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_NONBLOCK",    O_NONBLOCK);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_DSYNC",       O_DSYNC);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_ASYNC",       O_ASYNC);
    append_flag_to_str(&buf_cur, buf_end, &flags, "__O_DIRECT",    __O_DIRECT);
    // O_LARGEFILE is 0 on x86_64
    if (__O_LARGEFILE)
    {
        append_flag_to_str(&buf_cur, buf_end, &flags, "__O_LARGEFILE", __O_LARGEFILE);
    }
    if (append_flag_to_str(&buf_cur, buf_end, &flags, "__O_TMPFILE",   __O_TMPFILE) == -1)
    {
        append_flag_to_str(&buf_cur, buf_end, &flags, "O_DIRECTORY",   O_DIRECTORY);
    }
    
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_NOFOLLOW",    O_NOFOLLOW);
    append_flag_to_str(&buf_cur, buf_end, &flags, "__O_NOATIME",   __O_NOATIME);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_CLOEXEC",     O_CLOEXEC);
    append_flag_to_str(&buf_cur, buf_end, &flags, "O_SYNC",        O_SYNC);
    append_flag_to_str(&buf_cur, buf_end, &flags, "__O_PATH",      __O_PATH);

    printf("flags: %s\n", buf_begin);
    if (flags)
    {
        printf("Unknown flags: 0x%x\n", flags);
    }
    free((void*)buf_begin);
}

void print_mode(mode_t mode)
{
    printf("mode: 0%03o\n", mode);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file_path = argv[1];
    if (access(file_path, F_OK))
    {
        perror(file_path);
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    int flags = O_CLOEXEC | O_WRONLY | __O_TMPFILE | 1 << 4;
    print_open_flags(flags);

    mode_t mode = S_IWGRP | S_IXUSR;
    print_mode(mode);

    exit(EXIT_SUCCESS);
}
