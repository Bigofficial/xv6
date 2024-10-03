#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);
    char buf[] = {'b'};
    if(fork() != 0){
        //parent process
        write(p2c[1], buf, 1);
        read(c2p[0], buf, 1);
        printf("%d: received pong\n", getpid());
        wait(0); //wait for child process exit
    }else{
        read(p2c[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(c2p[1], buf, 1);
    }
    exit(0);
}