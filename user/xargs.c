#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MSGSIZE 16


// mkdir a
// echo hello > a/b
// mkdir c
// echo hello > c/b
// echo hello > b
// find . b | xargs grep hello

int 
main(int argc, char *argv[])
{
    sleep(10);
    // 如何得到前一个命令的标准化输出
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);


    // 如何得到自己的命令行参数
    char *xargv[MAXARG];
    int xargc = 0;
    for (int i = 1; i < argc; ++i)
    {
        xargv[xargc++] = argv[i];
    }

    char *p = buf;
    for (int i = 0; i < MSGSIZE; ++i)
    {
        if (buf[i] == '\n') //p-i 是下一个参数
        {
            int pid = fork();
            if (pid > 0)
            {
                p = &buf[i + 1];
                wait(0);
            }
            else
            {
                // 如何使用exec去执行命令
                buf[i] = 0;
                xargv[xargc] = p;
                xargc++;

                exec(xargv[0], xargv);
                exit(0);
            }
        }
    }
    wait(0);
    exit(0);
}


    // printf("获得到的标准化输入: %s\n", buf);

    // printf("获得到的argc: %d\n", argc);

    // for(int i = 0; i < argc; i++){
    //     printf("argv[%d]: %s\n", i, argv[i]);
    // }