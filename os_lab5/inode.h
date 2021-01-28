#ifndef INODE_H
#define INODE_H
#include <stdint.h>
#include "disk.h"
#define FILE_TYPE 0
#define DIR_TYPE 1
#define MAX_INODE_NUM 32   //1024/32
#define INODE_BLOCK_BEGIN 1

typedef struct inode {
    uint32_t size;              // 文件大小
    uint16_t file_type;         // 文件类型（文件/文件夹）
    uint16_t link;              // 连接数
    uint32_t block_point[6];    // 数据块指针
}inode_t;

char inode_block[2*DEVICE_BLOCK_SIZE];
uint32_t make_inode(uint32_t size, uint16_t file_type, uint32_t *block_point);
int read_inode(uint32_t inode_id, uint16_t *file_type, uint32_t *block_point, uint32_t *size);
int update_inode(uint32_t inode_id, uint32_t size_to_add);
int show_inode_content(uint32_t inode_id);

#endif