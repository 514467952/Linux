src=$(wildcard ./*.c)
obj=$(patsubst %.c,%.o,$(src))
.PHONY:child main
all: child main

child:$(obj)
	gcc $^  -o $@
main:$(obj)
	gcc $^  -o $@
%.o:%.c
	gcc -c $< -o $@
