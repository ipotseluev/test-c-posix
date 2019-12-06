#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <attr/xattr.h>

void usage(const char* prog_name)
{
    printf("Get xattrs for a given file\n");
    printf("Usage: %s FILE\n", prog_name);
}

void list_attrs(char *file_path)
{
    const int attr_buf_size = listxattr(file_path, NULL, 0);
    if (attr_buf_size == -1)
    {
        perror("Failed to get size of file attributes");
        exit(EXIT_FAILURE);
    }

    char attr_buf[attr_buf_size];
    bzero(attr_buf, attr_buf_size);
    printf("File %s has %d bytes of attributes\n", file_path, attr_buf_size);
    int ret = listxattr(file_path, attr_buf, attr_buf_size);
    printf("File attributes:\n%s\n", attr_buf);
}

void get_acl(char *file_path)
{
    const char attr_name[] = "system.posix_acl_access";
    const int attr_value_size = getxattr(file_path, attr_name, 0, 0);
    if (attr_value_size == -1)
    {
        if (errno == ENOATTR)
            fprintf(stderr, "File %s doesn't have attribute \"%s\"",
                    file_path, attr_name);
        else
            perror("getxattr(): ");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv)
{
    if (argc != 2) 
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    char *file_path = argv[1];
    if (access(file_path, F_OK))
    {
        fprintf(stderr, "File %s doesn't exist\n", file_path);
        exit(EXIT_FAILURE);
    }

    list_attrs(file_path);
    get_acl(file_path);

    exit(EXIT_SUCCESS);
}
