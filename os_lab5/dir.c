#include "dir.h"
#include "superblock.h"
#include "utils.h"
#include "inode.h"
#include "string.h"
#include <string.h>
#include <stdio.h>

uint32_t make_dir(char *dirname)
{
    char dir_block_temp[2 * DEVICE_BLOCK_SIZE];
    uint32_t block_num = get_a_free_block();
    memset(dir_block_temp, 0, 2 * DEVICE_BLOCK_SIZE * sizeof(char));
    dir_item_t *di = (dir_item_t *)dir_block_temp;
    strcpy(di[0].name, dirname);
    di[0].valid = 2;
    di[0].type = DIR_TYPE;
    uint32_t block_point[6];
    for (int i = 0; i < 6; i++)
    {
        block_point[i] = 0;
    }
    block_point[0] = block_num;
    uint32_t inode_id = make_inode(ONE_DIR_ITEM_LENGTH, DIR_TYPE, block_point);
    di[0].inode_id = inode_id;
    disk_write_data_block(block_num, dir_block_temp, DIR_BLOCK_BEGIN);

    sp_block *spb = (sp_block *)spb_block;
    spb->dir_inode_count++;
    return inode_id;
}

uint32_t make_file(char *filename)
{
    uint32_t block_num = get_a_free_block();
    uint32_t block_point[6];
    for (int i = 0; i < 6; i++)
    {
        block_point[i] = 0;
    }
    block_point[0] = block_num;
    //当前只为每个文件分配1个block大小的无内容空间
    disk_write_data_block(block_num, temp_data_block, DIR_BLOCK_BEGIN);
    //temp_data_block 是cp指令从上一个文件写来的，实际上这个块是空的，也只写了第0块
    uint32_t inode_id = make_inode(DEVICE_BLOCK_SIZE * 2, FILE_TYPE, block_point);
    return inode_id;
}

uint32_t find_dir_block(uint32_t block_num, char *name, uint16_t valid)
{
    disk_read_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
    dir_item_t *di = (dir_item_t *)dir_block;
    int i;
    for (i = 0; i < MAX_DIR_ITEM; i++)
    {
        if (!strcmp(di[i].name, name) && di[i].valid == valid)
        {
            break;
        }
    }
    if (i == MAX_DIR_ITEM)
    {
        return 0;
    }
    else
    {
        return di[i].inode_id;
    }
}

int insert_dir(char *dirname, uint32_t father_inode_id)
{
    uint16_t file_type;
    uint32_t block_point[6];
    uint32_t size;
    update_inode(father_inode_id, ONE_DIR_ITEM_LENGTH);
    read_inode(father_inode_id, &file_type, block_point, &size);
    int i = size / 1024;
    uint32_t block_num = block_point[i];
    disk_read_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
    dir_item_t *di = (dir_item_t *)dir_block;
    int k;
    for (k = 0; k < MAX_DIR_ITEM; k++)
    {
        if (di[k].valid == 0)
        {
            break;
        }
    }
    int inode_id = make_dir(dirname);
    di[k].inode_id = inode_id;
    strcpy(di[k].name, dirname);
    di[k].type = DIR_TYPE;
    di[k].valid = 1;
    disk_write_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
    return 0;
}

int find_dir(char *dirname, uint32_t father_inode_id)
{
    uint16_t file_type;
    uint32_t block_point[6];
    uint32_t size;
    read_inode(father_inode_id, &file_type, block_point, &size);
    uint32_t block_num;
    int finddirsign = 0;
    int k;
    dir_item_t *di = (dir_item_t *)dir_block;
    for (int i = 0; i < 6; i++)
    {
        block_num = block_point[i];
        if (block_num == 0)
        {
            break;
        }
        disk_read_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
        for (k = 0; k < MAX_DIR_ITEM; k++)
        {
            if (di[k].valid == 1 && strcmp(di[k].name, dirname) == 0 && di[k].type == DIR_TYPE)
            {
                finddirsign = 1;
                break;
            }
        }
        if (finddirsign)
        {
            break;
        }
    }
    return finddirsign;
}

int insert_file(char *filename, uint32_t father_inode_id)
{
    uint16_t file_type;
    uint32_t block_point[6];
    uint32_t size;
    update_inode(father_inode_id, ONE_DIR_ITEM_LENGTH);
    read_inode(father_inode_id, &file_type, block_point, &size);
    int i = size / 1024;
    uint32_t block_num = block_point[i];
    disk_read_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
    dir_item_t *di = (dir_item_t *)dir_block;
    int k;
    for (k = 0; k < MAX_DIR_ITEM; k++)
    {
        if (di[k].valid == 0)
        {
            break;
        }
    }
    int inode_id = make_file(filename);
    di[k].inode_id = inode_id;
    strcpy(di[k].name, filename);
    di[k].type = FILE_TYPE;
    di[k].valid = 1;
    disk_write_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
    return 0;
}

int find_file(char *filename, uint32_t father_inode_id)
{
    uint16_t file_type;
    uint32_t block_point[6];
    uint32_t size;
    dir_item_t *di = (dir_item_t *)dir_block;
    int k;
    int findfilesign = 0;
    read_inode(father_inode_id, &file_type, block_point, &size);
    uint32_t block_num;
    for (int i = 0; i < 6; i++)
    {
        block_num = block_point[i];
        if (block_num == 0)
        {
            break;
        }
        disk_read_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
        for (k = 0; k < MAX_DIR_ITEM; k++)
        {
            if (di[k].valid == 1 && strcmp(di[k].name, filename) == 0 && di[k].type == FILE_TYPE)
            {
                findfilesign = 1;
                break;
            }
        }
        if (findfilesign)
        {
            break;
        }
    }
    int inode_id = di[k].inode_id;
    read_inode(inode_id, &file_type, block_point, &size);
    disk_read_data_block(block_point[0], temp_data_block, DIR_BLOCK_BEGIN);  
    //将文件数据读入临时数据块，实际上读的都是空文件，并且只读了第0块，待优化
    return findfilesign;
}

int show_dir_block(uint32_t block_num, uint8_t type)
{
    disk_read_data_block(block_num, dir_block, DIR_BLOCK_BEGIN);
    dir_item_t *di = (dir_item_t *)dir_block;
    for (int i = 0; i < MAX_DIR_ITEM; i++)
    {
        if (di[i].valid == 1 && di[i].type == type)
        {
            if(type == DIR_TYPE) printf("dir\t");
            else if(type == FILE_TYPE) printf("file\t");
            printf("%s\n", di[i].name);
        }
    }
    return 0;
}
