##
## Makefile 
##

## Define compiler

CC	= gcc

## Define flags

CFLAGS	= -std=c99 -Wall -Wshadow -Werror -Wvla -g -pedantic -fstack-protector-strong --param ssp-buffer-size=1

## if you want to compile with the -O3 flag, comment the preceding CFLAGS and 
## uncomment the following CFLAGS

## CFLAGS	= -std=c99 -Wall -Wshadow -Werror -Wvla -O3 -pedantic -fstack-protector-strong --param ssp-buffer-size=1

## Build rule to build executable, by default, pe01 is build since it 
## is the label of the first rule

pe05: pa03.o answer03.o 
	$(CC) $(CFLAGS) pa03.o answer03.o -o pa03 -lm

## rule to test with argument option 0

test1: pa03
	./pa03 0 inputfile

## rule to test with argument option 1

test2: pe06
	./pa03 1 inputfile

test3: pa03
	./pa03

## rule to test all

test_all: test3 test1 test2

## rule to run valgrind

memory: pa03
	valgrind --log-file=log0.txt ./pa03 0 inputfile
	valgrind --log-file=log1.txt ./pa03 1 inputfile

## rule to clean up object files and executable so that you can rebuild

clean:	
	rm -f pa03.o answer03.o pa03

## rule to compile .c to .o

%.o: %.c
	$(CC) $(CFLAGS) -c $< 
