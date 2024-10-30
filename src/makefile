shell = mysh2
ls = myls

all: $(shell).bin

$(shell).bin: $(shell).o $(ls).o

%.o: %.c
	gcc $< -o ../bin/$(basename $<).bin -ansi

start: $(shell).bin
	../bin/$(shell).bin

format:
	clang-format -i *.c