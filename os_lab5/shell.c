#include "shell.h"
#include "filesystem.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>

int runshell()
{
    if(open_disk() == -1)  //打开磁盘
    {
        printf("\nFail to open disk!\n");
        return 0;
    }
    sign_shutdown = 0;
    init_sys();   //初始化系统
    while (!sign_shutdown)
    {
        printf(">> ");
        fgets(shellbuf, SHELLBUF_MAX_LEN, stdin);
        split();  //将用户的输入进行拆分为命令和参数
        runcmd(); //根据输入命令运行相应子程序
    }
    close_disk();  //关闭磁盘
}

void split()
{
    for (int k = 0; k < ARGV_MAX_NUM; k++) { argv[k][0] = 0; }

    int i;
    char temp[ARGV_MAX_LEN];
    int avgc = 0;
    int temp_p = 0;
    for ( i = 0; i < SHELLBUF_MAX_LEN; i++)
    {
        if (avgc > ARGV_MAX_NUM)
        {
            printf("Too many arguments.\n");
            break;
        }
        else if(shellbuf[i] == '\n')
        {
            if (temp_p) strcpy(argv[avgc], temp);
            break;
        }            
        else if (shellbuf[i] == ' ' && temp_p != 0)
        {
            strcpy(argv[avgc], temp);
            avgc++;
            temp_p = 0;
        }
        else if (shellbuf[i] != ' ')
        {
            temp[temp_p] = shellbuf[i];
            temp_p++;
            temp[temp_p] = '\0';
        }                
    }    
}

void runcmd()
{
    char *cmd = argv[0];
    if (!strcmp(cmd, "ls"))
    {
        ls(argv[1]);
    }
    else if (!strcmp(cmd, "mkdir"))
    {
        mkdir(argv[1]);
    }
    else if (!strcmp(cmd, "touch"))
    {
        touch(argv[1]);
    }
    else if (!strcmp(cmd, "cp"))
    {
        cp(argv[1], argv[2]);
    }
    else if (!strcmp(cmd, "shutdown"))
    {
        shutdwon();
        sign_shutdown = 1;
    }
    else
    {
        printf("'%s': command not found.\n", cmd);
    }    
}