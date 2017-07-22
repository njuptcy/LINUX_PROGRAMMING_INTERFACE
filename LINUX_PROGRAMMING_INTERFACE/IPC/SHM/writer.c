#include<sys/sem.h>
#include"shm.h"

int main()
{
    key_t key = ftok(FILENAME, 'x');
    int id;
    int flag = S_IRUSR|S_IWUSR|IPC_CREAT;
    ERRHANDLER(id = shmget(key,sizeof(struct shmBuf), flag ));
    struct shmBuf * pShmBuf;
    struct sembuf op;
    int semid;
    int send_times = 0;
    int bytes = 0;
    ERRHANDLER(semid = semget(key, 1, IPC_CREAT|S_IRUSR|S_IWUSR));
    pShmBuf = shmat(id, NULL, 0);

    //initialize the sem to 1
    /* op.sem_num = 0; */
    /* op.sem_op = 1; */
    /* op.sem_flg = 0; */
    /* ERRHANDLER(semop(semid, &op, 1)); */
    int init_arg = 1;
    semctl(semid, 0, SETVAL, init_arg);

    while(1)
    {
        op.sem_num = 0;
        op.sem_op = -1;
        op.sem_flg = SEM_UNDO;
        ERRHANDLER(semop(semid, &op, 1));
        pShmBuf->cnt = read(STDIN_FILENO, pShmBuf->data, MAXSIZE);
        op.sem_op = 1;
        send_times++;
        bytes += pShmBuf->cnt; 
        ERRHANDLER(semop(semid, &op, 1));
        if(pShmBuf->cnt == 0)
        {
            break;
        }
    }
    op.sem_num = 0;
    op.sem_flg = 0;
    op.sem_op = -1;
    ERRHANDLER(semop(semid, &op, 1));
    ERRHANDLER(semctl(semid, 0, IPC_RMID, NULL)); 
    ERRHANDLER(shmdt(pShmBuf));
    ERRHANDLER(shmctl(id, IPC_RMID, NULL));
}
