#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

struct __attribute__((__packed__)) superblock_t {
    uint8_t   fs_id [8];
    uint16_t block_size;
    uint32_t file_system_block_count;
    uint32_t fat_start_block;
    uint32_t fat_block_count;
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
};

void main(int argc, char* argv[]) {

    int fd = open(argv[1], O_RDWR);
    struct stat buffer;
    int status = fstat(fd, &buffer);

    //map address to a value
    void* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //print out superblock infomation
    struct superblock_t* sb;
    sb=(struct superblock_t*)address;
    printf("Super block information:\n");
    printf("Block size: %d\n",ntohs(sb->block_size));
    printf("Block count: %d\n", ntohl(sb->file_system_block_count));
    printf("FAT starts: %d\n", ntohl(sb->fat_start_block));
    printf("FAT blocks: %d\n", ntohl(sb->fat_block_count));
    printf("Root directory start: %d\n", ntohl(sb->root_dir_start_block));
    printf("Root directory blocks: %d\n", ntohl(sb->root_dir_block_count));

    //increment to FAT start
    address+=ntohs(sb->block_size)*ntohl(sb->fat_start_block);
    int avai_counter=0;
    int reser_counter=0;
    int allo_counter=0;
    for (int i =0;i<ntohl(sb->file_system_block_count);i++) 
    {
        //check the value of each entry to determine if it is available and increment counters
        int fssize;
        memcpy(&fssize, address, 4);
        fssize=ntohl(fssize);
        if (fssize==0)
        {
            avai_counter+=1;
        }else if (fssize==1)
        {
            reser_counter+=1;
        } else {
            allo_counter+=1;
        }
        
        address+=4;
    }
    printf("FAT information:\n");
    printf("Free Blocks: %d\n",avai_counter);
    printf("Reserved Blocks: %d\n",reser_counter);
    printf("Allocated Blocks: %d\n",allo_counter);
    

    munmap(address,buffer.st_size);
    close(fd);
}