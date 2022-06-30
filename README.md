#INTRODUCTION

-Microsoft's FAT file system can be read and operated on using this program. By traversing the FAT table this program acquire infomations needed to perform specific actions on the file system.

#INPUT

-A file system (a file with a .dmg or .img extension)
-./diskinfo take 1 argument which is the file system
-./disklist take 1 argument which is the file system
-./diskget take 3 argument: file system, file name to be found, file name of output file

#OUTPUT

-./diskinfo list all info of the file system and the FAT table
ie.
Super block information:
Block size: 512
Block count: 5120
FAT starts: 1
FAT blocks: 40
1
Root directory start: 41
Root directory blocks: 8
FAT information:
Free Blocks: 5071
Reserved Blocks: 41
Allocated Blocks: 8

-./disklist list all file and directory with F for file and D for directory, size, filename,creation date and time
ie.
F 2560 foo.txt 2005/11/15 12:00:00
F 5120 foo2.txt 2005/11/15 12:00:00
F 48127 makefs 2005/11/15 12:00:00
F 8 foo3.txt 2005/11/15 12:00:00

-./diskget copy a file from the file system and output it to the working directory

#HIGH-LEVEL OVERVIEW

./disklist
-creates a superblock struct with the info of the superblock which is the first block of the FAT system
-then loops through every entry in the system and increment the corresponding counter

./disklist
-create a superblock struct to get the root dir section
-loop through the root directory and create a directory entry block and list out the files and directory in root

./diskget
-create a superblock struct to get the blocksize and fat start
-loop through the root dir and search for the file, if found then we get the start block
-increment the memory to the start block in the FAT entries to get the next block number until we reach end of file
-also with each block number we increment another memory pointer to the correspsonding block and copy all the info to a specified output file
-repeat til the FAT entry is 0xFFFFFFFF which is end of file
