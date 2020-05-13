
CC = gcc
CFLAGS = -g -Wall -std=gnu99 -D_REENTRANT

SRCS = grid.c life.c
OBJS = $(SRCS:%.c=%.o)
MAINSRCS = main.c
PROGS = $(MAINSRCS:%.c=%)

all : grid life
	@echo "Creating all targets"
	$(CC) $(CFLAGS) -c -o $(PROGS).o $(MAINSRCS) -pthread
	$(CC) -o $(PROGS) main.o grid.o life.o -pthread

grid :
	$(CC) $(CFLAGS) -c -o $@.o grid.c

life :
	$(CC) $(CFLAGS) -c -o $@.o life.c

clean :
	rm -f *.o $(PROGS)
