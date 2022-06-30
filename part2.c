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

struct __attribute__((__packed__)) dir_entry_timedate_t {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};


// Directory entry
struct __attribute__((__packed__)) dir_entry_t {
    uint8_t status;
    uint32_t starting_block;
    uint32_t block_count;
    uint32_t size;
    struct dir_entry_timedate_t create_time;
    struct dir_entry_timedate_t modify_time;
    uint8_t filename[31];
    uint8_t unused[6];
};


void main(int argc, char* argv[]) {

    int fd = open(argv[1], O_RDWR);
    struct stat buffer;
    int status = fstat(fd, &buffer);

    //map memory to a value
    void* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    struct superblock_t* sb;
    sb=(struct superblock_t*)address;
    //increment address to root directory
    address+=ntohs(sb->block_size)*ntohl(sb->root_dir_start_block);

    //loop through each directory
    for (int i =0;i<ntohl(sb->root_dir_block_count);i++)
    {
        //start a directory struct at each directory
        struct dir_entry_t* entry;
        entry=(struct dir_entry_t*)address;
        int fssize;
        memcpy(&fssize, address, 4);

        //check the status, if 3 then file if 5 then dir
        if(entry->status==3){
            printf("F %10d %30s %04d/%02d/%02d %02d:%02d:%02d\n", ntohl(entry->size),(char*) entry->filename,ntohs(entry->create_time.year),entry->create_time.month,entry->create_time.day,entry->create_time.hour,entry->create_time.minute,entry->create_time.second);
        } else if (entry->status==5)
        {
            printf("D %10d %30s %04d/%02d/%02d %02d:%02d:%02d\n", ntohl(entry->size),(char*) entry->filename,ntohs(entry->create_time.year),entry->create_time.month,entry->create_time.day,entry->create_time.hour,entry->create_time.minute,entry->create_time.second);
        }
        address+=64;
    }
    
    

    munmap(address,buffer.st_size);
    close(fd);
}