shell = mysh

all: $(shell).bin

$(shell).bin: $(shell).o

%.o: %.c
	gcc $< -o ../bin/$(basename $<) -ansi

start: $(shell).bin
	../bin/$(shell).bin

format:
	clang-format -i *.c