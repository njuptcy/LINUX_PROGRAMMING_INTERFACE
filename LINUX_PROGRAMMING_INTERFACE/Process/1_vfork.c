#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main()
{
    int val = 222;
    char *str2 = "parent is running\n";
    char *str = "child is running\n";
    switch(vfork())
    {

        case -1:
            perror("vfork");
            exit(-1);

        case 0:
            val *= 2;
            write(STDOUT_FILENO,str, strlen(str));
            sleep(3);
            _exit(EXIT_SUCCESS);

        default:
            write(STDOUT_FILENO, str2, strlen(str2));
            printf("val in parent is:%d\n", val);
            exit(EXIT_SUCCESS);
    }
}
