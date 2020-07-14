#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char* line =  NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    printf("Your input: %s\n", line);
    return 0;
}
