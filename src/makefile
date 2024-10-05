all: mysh.bin

mysh.bin: mysh.o

%.o: %.c
	gcc $< -o ../bin/mysh.bin -ansi -std=c23

start: mysh.bin
	../bin/mysh.bin

format:
	clang-format -i *.c