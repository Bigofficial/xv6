#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//sleep 10
int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(2, "usage: sleep <ticks>\n");
        exit(1);
    }
    printf("sleeping...\n");
    sleep(atoi(argv[1]));
    exit(0);
}