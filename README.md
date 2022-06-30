# INTRODUCTION

- Microsoft's FAT file system can be read and operated on using this program. By traversing the FAT table this program acquire infomations needed to perform specific actions on the file system.

# INPUT

- A file system (a file with a .dmg or .img extension)
- ./diskinfo take 1 argument which is the file system
- ./disklist take 1 argument which is the file system
- ./diskget take 3 argument: file system, file name to be found, file name of output file

# OUTPUT

- ./diskinfo list all info of the file system and the FAT table
ie.
<img width="168" alt="image" src="https://user-images.githubusercontent.com/63203684/176587361-8fb7dcf8-afae-4ab1-9f49-8038ed59fac1.png">
<img width="183" alt="image" src="https://user-images.githubusercontent.com/63203684/176587397-f51c1adc-a1fe-49a2-8352-f9d609a0a81b.png">


- ./disklist list all file and directory with F for file and D for directory, size, filename,creation date and time
ie.
<img width="375" alt="image" src="https://user-images.githubusercontent.com/63203684/176587440-42f587a0-2ed6-485d-bb4d-abba9cefb54e.png">


- ./diskget copy a file from the file system and output it to the working directory

# HIGH-LEVEL OVERVIEW

./disklist
- creates a superblock struct with the info of the superblock which is the first block of the FAT system
- then loops through every entry in the system and increment the corresponding counter

./disklist
- create a superblock struct to get the root dir section
- loop through the root directory and create a directory entry block and list out the files and directory in root

./diskget
- create a superblock struct to get the blocksize and fat start
- loop through the root dir and search for the file, if found then we get the start block
- increment the memory to the start block in the FAT entries to get the next block number until we reach end of file
- also with each block number we increment another memory pointer to the correspsonding block and copy all the info to a specified output file
- repeat til the FAT entry is 0xFFFFFFFF which is end of file
