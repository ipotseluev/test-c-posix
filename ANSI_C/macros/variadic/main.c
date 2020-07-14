#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUT_EXTENSION(funcname, args...) \
    ({ \
    char str_buf[4096]; \
    char *str_cur = &str_buf[0]; \
    int str_written = 0;\
    const int microsec = 199; \
    str_written = snprintf(str_cur, 4096, "%s(", funcname); str_cur += str_written;\
    str_written = snprintf(str_cur, 4096 - str_written, args); str_cur += str_written;\
    str_written = snprintf(str_cur, 4096 - str_written, "): %d microsec\n", microsec); \
    printf("%s", str_buf); \
    })

#define OUT_STD(funcname, ...) \
    ({ \
    char str_buf[4096]; \
    char *str_cur = &str_buf[0]; \
    int str_written = 0;\
    const int microsec = 199; \
    str_written = snprintf(str_cur, 4096, "%s(", funcname); str_cur += str_written;\
    str_written = snprintf(str_cur, 4096 - str_written, __VA_ARGS__); str_cur += str_written;\
    str_written = snprintf(str_cur, 4096 - str_written, "): %d microsec\n", microsec); \
    printf("%s", str_buf); \
    })

#define OUT_STD_CUTE(funcname, format, ...) \
    ({ \
     if (format[0] == ']') { \
         char str_buf[4096]; \
         char *str_cur = &str_buf[0]; \
         int str_written = 0;\
         const int microsec = 111; \
         str_written = snprintf(str_cur, 4096, "%s(", funcname); str_cur += str_written;\
         str_written = snprintf(str_cur, 4096 - str_written, format, ##__VA_ARGS__); str_cur += str_written;\
         str_written = snprintf(str_cur, 4096 - str_written, "): %d microsec\n", microsec); \
         printf("%s", str_buf); \
     } \
     else { \
        printf("%s: %d microsec\n", funcname, 222); \
     } \
    })

#define OUT_WO_VA_ARGS(funcname) \
    ({ \
     printf("%s(): 55 microsec\n", funcname); \
    })

#define OUT_STD_CUTE_(funcname) OUT_STD_CUTE(funcname, "]")

#define CHOOSE_MACRO(_1, _2, _3, _4, MACRONAME, ...) MACRONAME
#define OUT(...) CHOOSE_MACRO(__VA_ARGS__, OUT_STD, OUT_STD, OUT_STD, OUT_WO_VA_ARGS)(__VA_ARGS__)

int main(int argc, char **argv)
{
    char str_buf[100];
    OUT_EXTENSION(__func__, "count=%d", 22);
    OUT_STD(__func__, "offset=%d", 33);
    OUT_STD_CUTE(__func__, "offset=%d", 44);
    OUT_STD_CUTE_(__func__);

    // OVERLOADING
    OUT(__func__);
    OUT(__func__, "overloaded with %c args", 'I');
    OUT(__func__, "overloaded with %c%c args", 'I', 'I');
    // OUT(__func__, "overloaded with %c%c%c args", 'I','I','I');
    exit(EXIT_SUCCESS);
}
