#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

typedef int bool;

/* lazy defines */
#define TRUE 1
#define FALSE 0

/* colors for debug */
#define BLANK      "\e[21;0m"
#define RED        "\e[1;31m"
#define GREEN      "\e[1;32m"
#define YELLOW     "\e[1;33m"
#define BLUE       "\e[1;34m"
#define MAGENTA    "\e[1;35m"
#define CYAN       "\e[1;36m"

#define INFO(format, ...) do {\
        printf("\t");\
        printf(GREEN);\
        printf(format, ##__VA_ARGS__);\
        printf(BLANK);\
        printf("\n");\
    }while(0)

#if defined(DEBUG)

    #define MISC(format, ...) do {\
            printf(CYAN);\
            printf(format, ##__VA_ARGS__);\
            printf(BLANK);\
            printf("\n");\
        }while(0)

    #define ERROR(format, ...) do {\
            printf(RED);\
            printf(format, ##__VA_ARGS__);\
            printf(BLANK);\
            printf("\n");\
        }while(0)
#else
    #define MISC(format, ...)
    #define ERROR(format, ...)
#endif

void error_handler(const char *);
void usage(char **);
long get_time(void);

#endif
