all:diskinfo disklist diskget

diskinfo: part1.c
	gcc part1.c -g -o diskinfo

disklist: part2.c
	gcc part2.c -g -o disklist

diskget: part3.c
	gcc part3.c -g -o diskget