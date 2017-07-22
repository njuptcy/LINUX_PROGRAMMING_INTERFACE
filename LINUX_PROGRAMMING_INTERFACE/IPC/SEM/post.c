#include"sem_header.h"

int main()
{
    key_t key = ftok(FILENAME, 'x');
    int id;
    int flag = IPC_CREAT|S_IRUSR|S_IWUSR;
    ERRHANDLER(id = semget(key,1,flag));
    struct sembuf op; 
    op.sem_flg = 0;
    op.sem_op = 1;
    op.sem_num = 0;
    while(1)
    {
        char ch = getchar();
        if(ch == 'e')
        {
            break;
        }
        printf("post a semephare\n");
        semop(id, &op, 1);
    }
    int s = semctl(id, 0, IPC_RMID);
    if(s == -1)
    {
        perror("semctl");
    }
    exit(EXIT_SUCCESS);
}
