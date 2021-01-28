#ifndef SHELL_H
#define SHELL_H

#define SHELLBUF_MAX_LEN 128
#define ARGV_MAX_NUM 4
#define ARGV_MAX_LEN 64

int sign_shutdown;
char shellbuf[SHELLBUF_MAX_LEN];
char argv[ARGV_MAX_NUM][ARGV_MAX_LEN];

void runcmd();
void split();
int runshell();

#endif