#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}



// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/fs.h"

// //找到最后一个/后面的文件名,将它格式化到DIRSIZ那么长,返回
// char*
// fmtname(char *path)
// {
//   static char buf[DIRSIZ+1];
//   char *p;

//   // Find first character after last slash.
//   for(p=path+strlen(path); p >= path && *p != '/'; p--)
//     ;
//   p++;

//   // Return blank-padded name.
//   if(strlen(p) >= DIRSIZ)
//     return p;
//   memmove(buf, p, strlen(p)); //copy
//   //填充空格,相当于格式化到DIRSIZ那么长
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
//   return buf;
// }

// void
// ls(char *path)
// {
//   char buf[512], *p;
//   int fd;
//   struct dirent de; //inode; name
//   struct stat st; //大小设备号组各种东西
//     //打开path
//   if((fd = open(path, 0)) < 0){
//     fprintf(2, "ls: cannot open %s\n", path);
//     return;
//   }
//     //文件具体信息到st
//   if(fstat(fd, &st) < 0){
//     fprintf(2, "ls: cannot stat %s\n", path);
//     close(fd);
//     return;
//   }
    
//   switch(st.type){
//   case T_DEVICE:
//   case T_FILE:
//   //文件,格式化打印 
//   // /path/to/file.txt
//     printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
//     break;

//   case T_DIR: //文件夹
//   //检查是否过长
//     if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
//       printf("ls: path too long\n");
//       break;
//     }
//     //路径复制到buf在末尾加上/
//     strcpy(buf, path);
//     p = buf+strlen(buf);
//     *p++ = '/';
//     // ./
//     //读取fd到de中
//     while(read(fd, &de, sizeof(de)) == sizeof(de)){
//       if(de.inum == 0) //空 未使用直接条多
//         continue;
//       memmove(p, de.name, DIRSIZ); //文件名复制到路径
//       p[DIRSIZ] = 0;
//       if(stat(buf, &st) < 0){ //获取元数据到st ./b
//         printf("ls: cannot stat %s\n", buf);
//         continue;
//       }
//       printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
//     }
//     break;
//   }
//   close(fd);
// }

// int
// main(int argc, char *argv[])
// {
//   int i;

//   if(argc < 2){
//     ls(".");
//     exit(0);
//   }
//   //ls a b 逐个查看a和b
//   for(i=1; i<argc; i++)
//     ls(argv[i]);
//   exit(0);
// }


