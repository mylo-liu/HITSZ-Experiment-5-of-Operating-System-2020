#ifndef UTILS_H
#define UTILS_H

int disk_write_data_block(unsigned int datablock_num, char* databuf, int begin_num);
int disk_read_data_block(unsigned int datablock_num, char* databuf, int begin_num);

#endif