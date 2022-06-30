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
    int found=0;

    //map address to a value
    void* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //get superblock info for block size
    struct superblock_t* sb;
    sb=(struct superblock_t*)address;
    //increment counter to the root dir to check each file name
    address+=ntohs(sb->block_size)*ntohl(sb->root_dir_start_block);
    for (int i =0;i<ntohl(sb->root_dir_block_count);i++)
    {
        //start a entry struct for each file
        struct dir_entry_t* entry;
        entry=(struct dir_entry_t*)(address+64*(i+1));
        int fssize;
        memcpy(&fssize, address, 4);
        //check if file name matches
        if(strcmp(entry->filename, argv[2])==0)
        {
            //start a file with the name in argument
            FILE *fp;
            found=1;
            fp = fopen( argv[3] , "w+" );
   

   
            //map a new address to copy file info
            void* faddress=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            //get the starting block memory position
            uint32_t ffssize;
            memcpy(&ffssize, &entry->starting_block, 4);
            
            for(;;)
            {
                //if end of file then exit
                if (ffssize==0xFFFFFFFF){
                    break;
                }
                //copy info from the block at each entry
                for (int j=0;j<(ntohs(sb->block_size));j++)
                {
                    int a;
                    memcpy(&a,faddress+(ntohs(sb->block_size)*ntohl(ffssize)-3)+j,4);
                    fprintf(fp,"%c",ntohl(a));
                    
                }
                //get the next block number
                memcpy(&ffssize, faddress+(ntohs(sb->block_size)*ntohl(sb->fat_start_block)+ntohl(ffssize)*4), 4);
                
                
                
            }

            fclose(fp);
            break;
        }
    }
    //if no file found output: "File not found"
    if (found==0)
    {
        printf("File not found.\n");
    }
    

    munmap(address,buffer.st_size);
    close(fd);
}