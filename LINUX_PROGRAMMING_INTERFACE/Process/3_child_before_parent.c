#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<wait.h>

static void handler(int sig)
{

}

int main()
{
    sigset_t blockMask, originMask, emptyMask;
    pid_t pid;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGUSR1);
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;

    setbuf(stdout, NULL);

    sigaction(SIGUSR1, &sa , NULL);
    if(sigprocmask(SIG_BLOCK,&blockMask, &originMask))
    {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    switch(pid = fork())
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            sigemptyset(&emptyMask);
            if(sigprocmask(SIG_BLOCK, &emptyMask, NULL) == -1)
            {
                perror("sigprocmask");
                exit(EXIT_FAILURE);
            }
            printf("child %ld is waitting for SIGUSR1\n", (long)getpid());
            if(sigsuspend(&emptyMask) == -1 && errno != EINTR)
            {
                perror("sigsuspend");
                exit(EXIT_FAILURE);
            }
            printf("child %ld has reveived SIGUSR1\n", (long)getpid());
            exit(21);
        default:
            printf("parent is running\n");
            sleep(3);
            printf("parent is about to send SIGUSR1 to child\n");
            if(kill(pid, SIGUSR1) == -1)
            {
                perror("kill");
                exit(EXIT_FAILURE);
            }
            int states;
            if(waitpid(pid, &states, 0) == -1 && errno != ECHILD)
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            if(WIFEXITED(states))
            {
                printf("child exit status %d.\n",WEXITSTATUS(states));
            }
            else if(WIFSIGNALED(states))
            {
                printf("the signal terminate child is :%d.\n", WTERMSIG(states));
            }
    }

}
