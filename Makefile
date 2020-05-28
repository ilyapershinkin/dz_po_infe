all:
	clang ./main.c ./books.c ./header.h ./functions.c ./students.c ./log.c -lm
	clear;
	-./a.out;
	rm ./a.out;
	clear;
clean:
	-rm ./a.out;
	clear;
