#include<sys/types.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>

#define FILENAME "sem_header.h"

#define MAXSIZE 1024
struct shmBuf
{
    int cnt;
    char data[MAXSIZE];
};

#define ERRHANDLER(exp) \
    if((exp) == -1)\
    {\
        perror(#exp);\
        exit(EXIT_FAILURE);\
    }while(0)

