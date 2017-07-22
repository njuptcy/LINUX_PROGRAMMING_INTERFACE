#include<sys/sem.h>
#include"shm.h"

int main()
{
    key_t key = ftok(FILENAME, 'x');
    int id;
    struct shmBuf * pShmBuf;
    int semid;
    struct sembuf op;
     ERRHANDLER(id = shmget(key, sizeof(struct shmBuf), S_IRUSR|S_IWUSR));
    op.sem_num = 0;
    op.sem_flg = SEM_UNDO;
    ERRHANDLER(semid = semget(key, 1, IPC_CREAT|S_IRUSR|S_IWUSR));
    pShmBuf = shmat(id, NULL, 0);

    while(1)
    {
        op.sem_op = -1;
        semop(semid, &op, 1); 
        if(pShmBuf->cnt == 0)
        {
            break;
        }
        write(STDOUT_FILENO, pShmBuf->data, pShmBuf->cnt);
        op.sem_op = 1;
        semop(semid, &op, 1);
    }
    shmdt(pShmBuf);

}
