#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define LS 648
#define MKDIR 649
#define TOUCH 650
#define CP 651

int init_sys();  //初始化
int ls(char *path);  //ls命令
int mkdir(char *path);  //mkdir命令
int touch(char *path);  //touch命令
int shutdwon();         //shutdown命令
int cp(char *path1, char *path2);  //cp命令
int findpath(char *path, int mode);  //路径寻找函数


#endif