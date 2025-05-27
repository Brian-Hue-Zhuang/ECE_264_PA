##
## Makefile 
##

## Define compiler

CC	= gcc

## Define flags

CFLAGS	= -std=c99 -Wall -Wshadow -Werror -Wvla -g -pedantic -fstack-protector-strong --param ssp-buffer-size=1

## if you want to compile with the -O3 flag, comment the preceding CFLAGS and 
## uncomment the following CFLAGS

#CFLAGS	= -std=c99 -Wall -Wshadow -Werror -Wvla -O3 -pedantic -fstack-protector-strong --param ssp-buffer-size=1

## Build rule to build executable, by default, pe01 is build since it 
## is the label of the first rule

pa02: pa02.o answer02.o 
	$(CC) $(CFLAGS) pa02.o answer02.o aux02.o utility02.o -o pa02 -lm

## rule to test with argument option 0

test1: pa02
	./pa02

## rule to test with argument option 1

## rule to run valgrind

memory: pa02
	valgrind --log-file=log0.txt ./pa02

## rule to clean up object files and executable so that you can rebuild

clean:	
	rm -f aux02.o utility02.o pa02.o answer02.o pa02

## rule to compile .c to .o

%.o: %.c
	$(CC) $(CFLAGS) -c $< 
