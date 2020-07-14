#include <stdio.h>

#define TEXT "text"

int main(int argc, char** argv)
{
#ifdef USER
    printf("defined\n");
#else
    printf("undefined\n");
#endif
    printf("Hello, %s\n", USER);
    printf("TEXT: %s\n", TEXT);
    return 0;
}
