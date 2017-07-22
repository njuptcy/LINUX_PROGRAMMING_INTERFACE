
#include"sem_header.h"

int main()
{
    key_t key = ftok(FILENAME, 'x');
    int flag = IPC_CREAT;
    int perm = S_IRUSR|S_IWUSR;
    int id;
    ERRHANDLER(id = semget(key, 1, flag|perm));

    struct sembuf sop;
    int n = 10;
    while(1)
    {
        sop.sem_num = 0;
        sop.sem_op = -1;
        sop.sem_flg = SEM_UNDO;
        if(semop(id, &sop, 1) != -1)
        {
            printf("the process %ld is printing a message\n", (long)getpid());
        }
        else
        {
            perror("semop");
            break;
        }
    }
}

