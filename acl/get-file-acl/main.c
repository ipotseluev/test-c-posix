#include <sys/types.h>
#include <sys/acl.h>
#include <acl/libacl.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h> // exit
#include <stdio.h>

void usage(char* prog_name)
{
    printf("Get access control list of a given file\n");
    printf("Usage: %s FILE\n", prog_name);
}

void tag_type_to_str(char* ret_name, size_t str_size, const acl_tag_t tag_type)
{
    assert(str_size > 18);

    switch (tag_type)
    {
        case ACL_UNDEFINED_TAG: strcpy(ret_name, "ACL_UNDEFINED_TAG"); break;
        case ACL_USER_OBJ:      strcpy(ret_name, "ACL_USER_OBJ"); break;
        case ACL_USER:          strcpy(ret_name, "ACL_USER"); break;
        case ACL_GROUP_OBJ:     strcpy(ret_name, "ACL_GROUP_OBJ"); break;
        case ACL_GROUP:         strcpy(ret_name, "ACL_GROUP"); break;
        case ACL_MASK:          strcpy(ret_name, "ACL_MASK"); break;
        case ACL_OTHER:         strcpy(ret_name, "ACL_OTHER"); break;
        default:                strcpy(ret_name, "WTF tag type"); break;
    }
}

void perms_to_str(char* out, size_t str_size, const acl_permset_t* permset)
{
    assert(str_size > 3);
    out[0] = acl_get_perm(*permset, ACL_READ) ? 'r' : '-';
    out[1] = acl_get_perm(*permset, ACL_WRITE) ? 'w' : '-';
    out[2] = acl_get_perm(*permset, ACL_EXECUTE) ? 'x' : '-';
    out[3] = '\0';
}

void qualifier_to_str(char* out,
                      size_t str_size,
                      const acl_entry_t entry,
                      const acl_tag_t tag_type)
{
    int ret;
    void *q = NULL;
    switch (tag_type)
    {
        case ACL_USER_OBJ:
        case ACL_GROUP_OBJ:
        case ACL_MASK:
        case ACL_OTHER:
            break;
        case ACL_USER:
            {
                q = acl_get_qualifier(entry);
                if (q == NULL)
                {
                    perror("Failed to obtain entry qualifier");
                    return;
                }
                ret = snprintf(out, str_size, " uid: %u", *((uid_t*)q));
                if (ret == -1)
                {
                    perror("Failed to write uid into string");
                    goto error;
                }
                break;
            }
        case ACL_GROUP:
            {
                q = acl_get_qualifier(entry);
                if (q == NULL)
                {
                    perror("Failed to obtain entry qualifier");
                    goto error;
                }
                snprintf(out, str_size, " gid: %u", *((gid_t*)q));
                if (ret == -1)
                {
                    perror("Failed to write gid into string");
                    goto error;
                }
                break;
            }
        default:
            goto error;
    }
    if (q)
        acl_free(q);
    return;

error:
    if (q)
        acl_free(q);
    sprintf(out, "Error");
    return;
}

int list_entries(const acl_t obj)
{
    acl_entry_t entry;
    int ret_entry = acl_get_entry(obj, ACL_FIRST_ENTRY, &entry);
    if (!ret_entry)
    {
        fprintf(stderr, "ERROR: File has no ACL entries");
        return -1;
    }

    while (ret_entry == 1)
    {
        char tag_name[100];
        char perms_str[4];
        char qualifier_str[100] = "";
        acl_perm_t perms = 0;

        acl_tag_t tag_type;
        int r = acl_get_tag_type(entry, &tag_type);
        if (r == -1)
        {
            perror("Failed to get entry tag type");
            return -1;
        }
        tag_type_to_str(tag_name, 100, tag_type);
        
        acl_permset_t permset;
        r  = acl_get_permset(entry, &permset);
        if (r == -1)
        {
            perror("Failed to get entry permset\n");
            return -1;
        }
        perms_to_str(perms_str, 4, &permset);

        qualifier_to_str(qualifier_str, 100, entry, tag_type);

        printf("%s %s%s\n", tag_name, perms_str, qualifier_str);
        ret_entry = acl_get_entry(obj, ACL_NEXT_ENTRY, &entry);
    }

    if (ret_entry == -1)
    {
        perror("Failed to get acl entry from acl object");    

    }

    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE); 
    }
    char *file_path = argv[1];

    acl_t acl_obj = acl_get_file(file_path, ACL_TYPE_ACCESS);
    if (acl_obj == NULL)
    {
        perror("Failed to obtain file access list: ");
        return EXIT_FAILURE;
    }
    int ret = list_entries(acl_obj);
    acl_free((void*)acl_obj);

    exit(EXIT_SUCCESS);
}
