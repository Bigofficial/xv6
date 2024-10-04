#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"


//主要注意 文件名字格式化的问题, ls是全部统一成DIRSIZ那么长,这里不是,我们就是截出/filename.txt还有后面的0

//找到最后一个/后面的文件名,将它格式化到DIRSIZ那么长,返回
char*
fmtname(char *path)
{
static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;
  memmove(buf, p, strlen(p)+1);
  return buf;
}

void eq_print(char* fileName, char * findName){
    if(strcmp(fmtname(fileName), findName) == 0){
        printf("%s\n", fileName);
    }
}

void find(char* path, char * findName){
    int fd;
    struct stat st;
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    char buf[512], *p;
    struct dirent de;
    switch (st.type)
    {
    case T_FILE:
        eq_print(path, findName);
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1  > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || de.inum == 1 || strcmp(de.name, ".")==0 || strcmp(de.name, "..")==0){
                continue;
            }
            memmove(p, de.name, strlen(de.name));
            p[strlen(de.name)] = 0;
            find(buf, findName);
        }
        break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    printf("find: find <path> <fileName>\n");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
