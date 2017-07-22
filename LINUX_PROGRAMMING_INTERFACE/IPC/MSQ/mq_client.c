#include"msq_header.h"


volatile sig_atomic_t FLAG = 1;
int GLOB_ID;

static void handler(int sig)
{
    printf("has reveived signal %d\n", sig);
    int s = msgctl(GLOB_ID, IPC_RMID, NULL);
    if(s == -1)
    {
        perror("msgctl");
    }
    //printf("has received sig %d\n", sig);
    FLAG = 0;
}

int main()
{
    key_t key = ftok(FILENAME, 'x');
    int id;
    int perm = S_IRUSR|S_IWUSR;
    int flags = IPC_CREAT;
    struct buf msg;
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    msg.mtype = 1;
    int len;
    sigaction(SIGINT, &sa, NULL);
    ERRHANDLER(id = msgget(key, flags|perm));
    GLOB_ID = id;
    setbuf(stdout, NULL);
    while(FLAG)
    {
        scanf("%s", msg.mtext);
        //msgsnd return 0 on success
        len = msgsnd(id, &msg, 
                strlen(msg.mtext)+1,0);
        printf("has send %d bytes\n", len);
    }
    printf("the client is about to exit\n");
    exit(EXIT_SUCCESS);
}
