#include<stdio.h>
#include<stdlib.h>
void func1()
{
    printf("func1 has been called\n");
}
void func2()
{
    printf("func2 has been called\n");
}
void func3()
{
    printf("func3 has been called\n");
}

void func4(int status, void *arg)
{
    printf("exit status is %d\n", status);
    printf("the arg pass in is:%s\n", (char*)arg);
}
int main()
{
    setbuf(stdout, NULL);
    atexit(func1);
    atexit(func2);
    atexit(func3);
    char buf[10] = "hello";
    on_exit(func4, buf);
    exit(EXIT_SUCCESS);
    //exit(EXIT_SUCCESS);
}
