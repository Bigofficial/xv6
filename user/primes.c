#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//pleft -- stage -- pright
void sieve(int pleft[2]){
    int p;
    read(pleft[0], &p, sizeof(p));
    if(p == -1){
        //finished
        exit(0);
    }
    printf("prime %d\n", p);
    int pright[2];
    pipe(pright); //输出到下一阶段

    if(fork() == 0){
        //子进程
        close(pright[1]); //无需写pright,我只要读当前stage给我的
        close(pleft[0]); //无需读pleft了
        sieve(pright);
    }else{
        close(pright[0]); //我只需给下一阶段写
        int buf;
        while(read(pleft[0], &buf, sizeof(buf))){
            if(buf % p != 0){
                write(pright[1], &buf, sizeof(buf));
            }
        }
        buf = -1; //还要-1, 最开始的进程的那个-1被第二个进程消化.
        write(pright[1], &buf, sizeof(buf));
        close(pright[1]);
        close(pleft[0]);
        wait(0);
        exit(0);
    }

}
int main(int argc, char*argv[]){
    int input_pipe[2];
    pipe(input_pipe);

    if(fork() == 0){
        //child process
        close(input_pipe[1]);
        sieve(input_pipe);
        exit(0);
    }else{
        //parent process
        close(input_pipe[0]);//parent just write
        int i;
        for(int i = 2; i <= 35; i++){
            write(input_pipe[1], &i, sizeof(i));
        }
        i = -1;
        write(input_pipe[1], &i, sizeof(i));
        close(input_pipe[1]);
    }
    wait(0);
    exit(0);
}