#include "utils.h"
#include "disk.h"


int disk_write_data_block(unsigned int datablock_num, char* databuf, int begin_num)
{
    unsigned int block_num0 = (datablock_num + begin_num) * 2;
    unsigned int block_num1 = block_num0 + 1;
    char *buf0 = databuf;
    char *buf1 = databuf + DEVICE_BLOCK_SIZE;
    disk_write_block(block_num0, buf0);
    disk_write_block(block_num1, buf1);
    return 0;
}

int disk_read_data_block(unsigned int datablock_num, char* databuf, int begin_num)
{
    unsigned int block_num0 = (datablock_num + begin_num) * 2;
    unsigned int block_num1 = block_num0 + 1;
    char *buf0 = databuf;
    char *buf1 = databuf + DEVICE_BLOCK_SIZE;
    disk_read_block(block_num0, buf0);
    disk_read_block(block_num1, buf1);
    return 0;
}