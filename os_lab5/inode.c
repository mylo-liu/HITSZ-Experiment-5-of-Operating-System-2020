#include "inode.h"
#include "utils.h"
#include "superblock.h"
#include "dir.h"
#include <stdio.h>

uint32_t make_inode(uint32_t size, uint16_t file_type, uint32_t *block_point)
{
    uint32_t inode_id = get_a_free_inode();
    disk_read_data_block(inode_id / 32, inode_block, INODE_BLOCK_BEGIN);
    inode_t *in = (inode_t *)inode_block;
    int i = inode_id % 32;
    in[i].size = size;
    in[i].file_type = file_type;
    for (int k = 0; k < 6; k++)
    {
        in[i].block_point[k] = block_point[k];
    }
    disk_write_data_block(inode_id / 32, inode_block, INODE_BLOCK_BEGIN);
    return inode_id;
}

int read_inode(uint32_t inode_id, uint16_t *file_type, uint32_t *block_point, uint32_t *size)
{
    disk_read_data_block(inode_id / 32, inode_block, INODE_BLOCK_BEGIN);
    inode_t *in = (inode_t *)inode_block;
    int i = inode_id % 32;
    (*file_type) = in[i].file_type;
    for (int k = 0; k < 6; k++)
    {
        block_point[k] = in[i].block_point[k];
    }
    *size = in[i].size;
    return 0;
}

int update_inode(uint32_t inode_id, uint32_t size_to_add)
{
    disk_read_data_block(inode_id / 32, inode_block, INODE_BLOCK_BEGIN);
    inode_t *in = (inode_t *)inode_block;
    int i = inode_id % 32;
    int block_point_num0 = in[i].size / 1024;
    int block_point_num1 = (in[i].size + size_to_add) / 1024;
    uint32_t add_block_num;
    if (block_point_num1 > block_point_num0)
    {
        add_block_num = get_a_free_block();
        in[i].block_point[block_point_num1] = add_block_num;
    }
    in[i].size += size_to_add;
    disk_write_data_block(inode_id / 32, inode_block, INODE_BLOCK_BEGIN);
    return 0;
}

int show_inode_content(uint32_t inode_id)
{
    uint16_t inode_file_type;  
    uint32_t block_point[6];
    uint32_t inode_size;
    read_inode(inode_id, &inode_file_type, block_point, &inode_size);
    int i;
    for (i = 0; i < 6; i++)
    {
        if (block_point[i] != 0)
        {
            show_dir_block(block_point[i], DIR_TYPE);
        }
    }
    for (i = 0; i < 6; i++)
    {
        if (block_point[i] != 0)
        {
            show_dir_block(block_point[i], FILE_TYPE);
        }
    }
    return 0;
}
