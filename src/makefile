all: mysh.bin

mysh.bin: mysh.o

%.o: %.c
	gcc $< -o ../bin/mysh.bin -ansi

start: mysh.bin
	../bin/mysh.bin

format:
	clang-format -i *.c