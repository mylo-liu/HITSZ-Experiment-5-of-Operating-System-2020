#include "filesystem.h"
#include "superblock.h"
#include "inode.h"
#include "dir.h"
#include <string.h>
#include <stdio.h>

int init_sys()
{
    read_spb();  //将超级块读入内存
    if (firstuse == 1) //初次使用，创建根目录
    {
        char dirname[] = "/";
        make_dir(dirname);
    }
}

int ls(char *path)
{
    if (strcmp(path, "/") == 0 || path[0] == 0)
    {
        show_inode_content(2);
    }
    else
    {
        findpath(path, LS);
    }
}

int mkdir(char *path)
{
    if (strcmp(path, "/") == 0 || path[0] != '/')
    {
        printf("\nInput dir path Error!\n\n");
        return 0;
    }
    else
    {
        findpath(path, MKDIR);
    }
}

int touch(char *path)
{
    if (path[0] != '/')
    {
        printf("\nPath Error!\n\n");
        return 0;
    }
    else
    {
        findpath(path, TOUCH);
    }
}

int cp(char *path1, char *path2)
{
    if (path1[0] != '/' || path2[0] != '/')
    {
        printf("\nPath Error!\n\n");
        return 0;
    }
    else
    {
        if (findpath(path1, CP))
        {
            touch(path2);
        }
        else
        {
            printf("\nFile '%s' do not exist!\n\n", path1);
        }
    }
}

int shutdwon()
{
    write_spb();
    return 0;
}

int findpath(char *path, int mode)
{
    char *start = path;
    char *end = path;
    uint32_t inode_id = 2;
    uint32_t block_point[6];
    uint16_t inode_file_type;
    uint32_t inode_size;
    while (1)
    {
        end++;
        start = end;
        while (1)
        {
            if (*end == '/' || *end == '\0')
            {
                break;
            }
            end++;
        }
        if (*end == '/')
        {
            *end = '\0';
            read_inode(inode_id, &inode_file_type, block_point, &inode_size);
            int i;
            for (i = 0; i < 6; i++)
            {
                if (block_point[i] != 0)
                {
                    inode_id = find_dir_block(block_point[i], start, 1);
                    if (inode_id != 0)
                    {
                        break;
                    }
                }
            }
            if (i == 6 && inode_id == 0)
            {
                printf("\nPath Error!\n\n");
                return 0;
            }
        }
        else if (*end == '\0')
        {
            if (mode == MKDIR)
            {
                if (find_dir(start, inode_id))
                {
                    printf("\nThe same name dir has existed! \n\n");
                }
                else
                {
                    insert_dir(start, inode_id);
                }
            }
            else if (mode == LS)
            {
                read_inode(inode_id, &inode_file_type, block_point, &inode_size);
                int i;
                for (i = 0; i < 6; i++)
                {
                    if (block_point[i] != 0)
                    {
                        inode_id = find_dir_block(block_point[i], start, 1);
                        if (inode_id != 0)
                        {
                            break;
                        }
                    }
                }
                if (i == 6 && inode_id == 0)
                {
                    printf("\nPath Error!\n\n");
                    return 0;
                }
                show_inode_content(inode_id);
            }
            else if (mode == TOUCH)
            {
                if (find_file(start, inode_id))
                {
                    printf("\nThe same name file has existed! \n\n");
                }
                else
                {
                    insert_file(start, inode_id);
                }
            }
            else if (mode == CP)
            {
                return find_file(start, inode_id);
            }
            break;
        }
    }
}