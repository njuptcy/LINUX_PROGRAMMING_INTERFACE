
#include<unistd.h>
#include"msq_header.h"

volatile sig_atomic_t FLAG = 1;

/* struct buf */
/* { */
/*     long mtype; */
/*     char mtext[MAX_SIZE]; */
/* }; */
int GLOB_ID;

static void handler(int sig)
{
    printf("has receive a SIGINT.\n");
    FLAG = 0;
    int s = msgctl(GLOB_ID, IPC_RMID, 0);
    if(s == -1)
    {
        perror("msgctl");
    }
}

int main()
{
    key_t key = ftok(FILENAME, 'x');
    int perm = S_IRUSR|S_IWUSR;
    int flag = IPC_CREAT;
    struct buf msg;
    int id;
    int len;
    struct sigaction sa;
    /* sigset_t blockMask, originMask; */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    sigaction(SIGINT, &sa, NULL);

    ERRHANDLER(id = msgget(key, flag|perm));
    GLOB_ID = id;

    while(FLAG)
    {
         len = msgrcv(id, &msg, MAX_SIZE, 0, 0);
         if(len == -1)
         {
             perror("msgrcv");
             sleep(1);
             printf("the server is about to exit\n");
         }
         else
         {
             printf("revieve length:%d, message is:%s\n",len, msg.mtext);
         }
    }
    printf("bye bye\n");
    exit(EXIT_SUCCESS);
}
