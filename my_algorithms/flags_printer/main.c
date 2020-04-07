#include "samba_flags.h"
#include <stdio.h>
#include <stdint.h>

struct flags_map_uint32_t
{
    uint32_t flag;
    const char name[255];
};

int print_flags_uint32(char** cur, const char* end,
                       uint32_t flags,
                       struct flags_map_uint32_t* flags_map,
                       size_t map_size)
{
    int n;
    const char* begin = *cur;
    for (int i = 0; i < map_size; i++)
    {
        if (flags & flags_map[i].flag)
        {
            n = snprintf(*cur, end - *cur, "%s ", flags_map[i].name);
            *cur += n;
        }
    }
    return *cur - begin;
}

int print_enum_uint32(char** cur, const char* end,
                      uint32_t input_value,
                      struct flags_map_uint32_t* flags_map,
                      size_t map_size)
{
    int n;
    for (int i = 0; i < map_size; i++)
    {
        if (input_value == flags_map[i].flag)
        {
            n = snprintf(*cur, end - *cur, "%s", flags_map[i].name);
            *cur += n;
            return n;

        }
    }
    return 0;
}

int main(int c, char** argv)
{
    {
        struct flags_map_uint32_t map_access[] =
        {
            {DELETE_ACCESS, "DELETE_ACCESS"},
            {READ_CONTROL_ACCESS, "READ_CONTROL_ACCESS"},
            {WRITE_DAC_ACCESS, "WRITE_DAC_ACCESS"},
            {WRITE_OWNER_ACCESS, "WRITE_OWNER_ACCESS"},
            {SYNCHRONIZE_ACCESS, "SYNCHRONIZE_ACCESS"},
            {SYSTEM_SECURITY_ACCESS, "SYSTEM_SECURITY_ACCESS"},
            {MAXIMUM_ALLOWED_ACCESS, "MAXIMUM_ALLOWED_ACCESS"},
            {GENERIC_ALL_ACCESS, "GENERIC_ALL_ACCESS"},
            {GENERIC_EXECUTE_ACCESS, "GENERIC_EXECUTE_ACCESS"},
            {GENERIC_WRITE_ACCESS, "GENERIC_WRITE_ACCESS"},
            {GENERIC_READ_ACCESS, "GENERIC_READ_ACCESS"}
        };

        size_t map_size = sizeof(map_access) / sizeof(struct  flags_map_uint32_t);
        printf("sizeof(map_access) = %zu\n", map_size);

        char buf[1024];
        char* cur = buf;
        const char* end = buf + 1024;

        uint32_t input = GENERIC_ALL_ACCESS | GENERIC_EXECUTE_ACCESS;
        int r = print_flags_uint32(&cur, end, input, map_access, map_size); 

        printf("%s\n", buf);
    }
    
    /* Testing enum */
    {
        struct flags_map_uint32_t map_disposition_enum[] =
        {
            {FILE_SUPERSEDE, "FILE_SUPERSEDE"},
            {FILE_OPEN, "FILE_OPEN"},
            {FILE_CREATE, "FILE_CREATE"},
            {FILE_OPEN_IF, "FILE_OPEN_IF"},
            {FILE_OVERWRITE, "FILE_OVERWRITE"},
            {FILE_OVERWRITE_IF, "FILE_OVERWRITE_IF"}
        };

        size_t map_size = sizeof(map_disposition_enum) / sizeof(struct  flags_map_uint32_t);
        printf("sizeof(map_access) = %zu\n", map_size);

        char buf[1024];
        char* cur = buf;
        const char* end = buf + 1024;

        uint32_t input = FILE_SUPERSEDE;
        int r = print_enum_uint32(&cur, end, input, map_disposition_enum, map_size);

        printf("%s\n", buf);
        return 0;
    }
}
