#ifndef DIR_H
#define DIR_H
#include <stdint.h>
#include "disk.h"
#define MAX_DIR_ITEM 8   //1024/128
#define ONE_DIR_ITEM_LENGTH 128
#define FILE_TYPE 0
#define DIR_TYPE 1
#define DIR_BLOCK_BEGIN 33

typedef struct dir_item {               // 目录项一个更常见的叫法是 dirent(directory entry)
    uint32_t inode_id;          // 当前目录项表示的文件/目录的对应inode
    uint16_t valid;             // 当前目录项是否有效 
    uint8_t type;               // 当前目录项类型（文件/目录）
    char name[121];             // 目录项表示的文件/目录的文件名/目录名
}dir_item_t;

char dir_block[2*DEVICE_BLOCK_SIZE]; //dirblock in memory, size is 1024
char temp_data_block[2*DEVICE_BLOCK_SIZE]; //dirblock in memory, size is 1024
uint32_t make_dir(char *dirname);
uint32_t find_dir_block(uint32_t block_num, char *name, uint16_t valid);
int insert_dir(char *dirname, uint32_t father_inode_id);
int show_dir_block(uint32_t block_num, uint8_t type);
int insert_file(char *filename, uint32_t father_inode_id);
uint32_t make_file(char *filename);
int find_file(char *filename, uint32_t father_inode_id);
int find_dir(char *dirname, uint32_t father_inode_id);

#endif