#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H
#include <stdint.h>
#include "disk.h"
#define MAGIC_NUM 0xdec0de

typedef struct super_block {
    int32_t magic_num;                  // 幻数
    int32_t free_block_count;           // 空闲数据块数
    int32_t free_inode_count;           // 空闲inode数
    int32_t dir_inode_count;            // 目录inode数
    uint32_t block_map[128];            // 数据块占用位图
    uint32_t inode_map[32];             // inode占用位图
} sp_block;

char spb_block[2*DEVICE_BLOCK_SIZE]; //superblock in memory, size is 1024
int firstuse;

int read_spb();
int init_spb();
uint32_t get_a_free_block();
uint32_t get_a_free_inode();
int write_spb();
int deal_uint32_bit(uint32_t n);

#endif