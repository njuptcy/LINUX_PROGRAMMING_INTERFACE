#include<signal.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define SYNC_SIG SIGUSR1
static void handler(int sig)
{

}


int main()
{
    sigset_t blockMask,oldMask,emptyMask; 
    struct sigaction sa;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);
    pid_t pid;

    setbuf(stdout, NULL);

    if(sigprocmask(SIG_BLOCK, &blockMask, &oldMask)== -1)
    {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;

    if(sigaction(SYNC_SIG, &sa, NULL) ==  -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    switch(pid = fork())
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            printf("child %ld is running\n", 
                    (long)getpid());
            sleep(3);
            printf("child is about to signal parent\n");
            if(kill(getppid(), SYNC_SIG) ==  -1 )
            {
                perror("kill");
                exit(EXIT_FAILURE);
            }
            sleep(1);
            printf("after signal parent, child continue.\n");
        default:
            sigemptyset(&emptyMask);
            if(sigsuspend(&emptyMask) == -1 && errno != EINTR)
            {
                perror("sigsuspend");
                exit(EXIT_FAILURE);
            }
            printf("parent is begin to run\n");
            printf("parent is about to end\n");
            exit(EXIT_SUCCESS);
    }

    

}
