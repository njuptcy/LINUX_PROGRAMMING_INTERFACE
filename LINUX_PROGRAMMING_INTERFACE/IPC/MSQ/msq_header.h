#include<signal.h>
#include<sys/stat.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>

#define ERRHANDLER(exp) \
    if((exp) == -1)\
    {\
        perror(#exp);\
        exit(EXIT_FAILURE);\
    }while(0)

#define MAX_SIZE 1024

#define FILENAME "msq_header.h"


struct buf
{
    long mtype;
    char mtext[MAX_SIZE];
};

