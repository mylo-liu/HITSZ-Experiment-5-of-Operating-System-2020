#include "superblock.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
#define MAX_BLOCK_COUNT 4063
#define MAX_INODE_COUNT 1024

int read_spb()
{
    sp_block *spb = (sp_block *)spb_block;
    disk_read_data_block(0, spb_block, 0);
    if (spb->magic_num == MAGIC_NUM)
    {
        printf("\nWelcome back!\n\n");
        firstuse = 0;
    }
    else
    {
        printf("\nFirst use the file system, init superblock!\n\n");
        firstuse = 1;
        init_spb();
        return 0;
    }
}
int write_spb()
{
    sp_block *spb = (sp_block *)spb_block;
    disk_write_data_block(0, spb_block, 0);
}

int init_spb()
{
    sp_block *spb = (sp_block *)spb_block;
    char *buf0 = spb_block;
    char *buf1 = spb_block + DEVICE_BLOCK_SIZE;
    spb->magic_num = MAGIC_NUM;
    spb->free_block_count = MAX_BLOCK_COUNT;
    spb->free_inode_count = MAX_INODE_COUNT;
    spb->dir_inode_count = 0;
    for (int i = 0; i < 128; i++) { spb->block_map[i] = 0; }
    for (int i = 0; i < 32; i++) { spb->inode_map[i] = 0; }
    spb->block_map[0] = 0xc0000000; //block0, block1 do not use
    spb->inode_map[0] = 0xc0000000; //inode0, inode1 do not use
    disk_write_block(0, buf0);
    disk_write_block(1, buf1);
    return 0;
}

uint32_t get_a_free_block()
{
    sp_block *spb = (sp_block *)spb_block;
    int i,j;
    for (i = 0; i < 128; i++)
    {
        if (spb->block_map[i] != 0xffffffff)
        {
            break;
        }        
    }
    j = deal_uint32_bit(spb->block_map[i]);
    uint32_t m = 0x80000000;
    m = m >> j;
    spb->block_map[i] = spb->block_map[i] | m;
    spb->free_block_count--;
    return i*32+j;
}

uint32_t get_a_free_inode()
{
    sp_block *spb = (sp_block *)spb_block;
    int i,j;
    for (i = 0; i < 32; i++)
    {
        if (spb->inode_map[i] != 0xffffffff)
        {
            break;
        }        
    }
    j = deal_uint32_bit(spb->inode_map[i]);
    uint32_t m = 0x80000000;
    m = m >> j;
    spb->inode_map[i] = spb->inode_map[i] | m;
    spb->free_inode_count--;
    return i*32+j;
}

int deal_uint32_bit(uint32_t n)
{
    int i;
    uint32_t m = 0x80000000;
    for (i = 0; i < 32; i++)
    {
        if ((m&n) == 0)
        {
            break;
        }
        else
        {
            m = m>>1;
        }        
    }
    return i;
}