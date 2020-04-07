#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

void usage(char* prog_name) {
    printf("Opens file\n");
    printf("Usage: %s FLAGS_HEX FILE\n", prog_name);
}

void print_open_flags(int flags)
{
    char* buf_cur = malloc(1024);
    assert(buf_cur);
    printf("Interpretation of open flags 0x%x\n", flags);

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

    bool wronly = !append_flag_to_str(&buf_cur, buf_end, &flags, "O_WRONLY", O_WRONLY);
    bool rdwr = !append_flag_to_str(&buf_cur, buf_end, &flags, "O_RDWR", O_RDWR);

    if (!wronly && !rdwr)
    {
        // O_RDONLY is a very special flag, it is 0 (aka 'not O_WRONLY and not O_RDWR')
        int written = snprintf(buf_cur, buf_end - buf_cur, "%s ", "O_RDONLY");
        assert(written != -1);
        buf_cur += written;
    }

    append_flag_to_str(&buf_cur, buf_end, &flags, "O_SYNC",        O_SYNC);
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

void print_flags_map()
{
    printf("Known flags map:\n");
    void print_flag(const char* name, int val)
    {
        printf("%-14s0x%x\n", name, val);
    }

    print_flag("O_RDONLY"    , O_RDONLY);
    print_flag("O_WRONLY"    , O_WRONLY);
    print_flag("O_RDWR"      , O_RDWR);
    print_flag("O_ACCMODE"   , O_ACCMODE);
    print_flag("O_CREAT"     , O_CREAT);
    print_flag("O_EXCL"      , O_EXCL);
    print_flag("O_NOCTTY"    , O_NOCTTY);
    print_flag("O_TRUNC"     , O_TRUNC);
    print_flag("O_APPEND"    , O_APPEND);
    print_flag("O_NONBLOCK"  , O_NONBLOCK);
    print_flag("O_DSYNC"     , O_DSYNC);
    print_flag("O_ASYNC"     , O_ASYNC);
#ifdef O_DIRECT
    print_flag("O_DIRECT"	   , O_DIRECT);
#endif
    print_flag("__O_LARGEFILE", __O_LARGEFILE);
#ifdef O_LARGEFILE
    print_flag("O_LARGEFILE" , O_LARGEFILE);
#endif
    print_flag("O_DIRECTORY" , O_DIRECTORY);
    print_flag("O_NOFOLLOW"  , O_NOFOLLOW);
#ifdef O_NOATIME
    print_flag("O_NOATIME"   , O_NOATIME);
#endif
    print_flag("O_CLOEXEC"   , O_CLOEXEC);
    print_flag("O_SYNC"      , O_SYNC);
#ifdef O_PATH
    print_flag("O_PATH"      , O_PATH);
#endif
#ifdef O_TMPFILE
    print_flag("O_TMPFILE"   , O_TMPFILE);
#endif
    printf("\n");
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

    print_flags_map();
    // int flags = 0x20002;
    int flags = 0x1210c2;
    print_open_flags(flags);

    mode_t mode = S_IWGRP | S_IXUSR;
    print_mode(mode);

    int fd = open(file_path, flags, mode);
    if (fd == -1)
    {
        perror("Open failed");
        exit(EXIT_FAILURE);
    }

    printf("File is opened. Press any key to finish\n");
    getchar();

    exit(EXIT_SUCCESS);
}
