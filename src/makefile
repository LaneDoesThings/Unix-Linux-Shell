shell = mysh2
ls = myls

all: $(shell).bin $(ls).bin

$(shell).bin: $(shell).o $(ls).o
$(ls).bin: $(ls).o


%.o: %.c
	gcc $< -o ../bin/$(basename $<).bin -ansi

start: $(shell).bin
	../bin/$(shell).bin

ls: $(ls).bin
	../bin/$(ls).bin

format:
	clang-format -i *.c