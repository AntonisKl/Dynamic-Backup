CC = gcc
CFLAGS  = -g -Wall
RM = rm -rf

default: mirr

mirr:  mirr.o tree.o utils.o stack.o 
	$(CC) $(CFLAGS) -o executables/mirr mirr.o tree.o utils.o stack.o 

mirr.o:  mirr.c
	$(CC) $(CFLAGS) -c mirr.c

tree.o:  tree/tree.c
	$(CC) $(CFLAGS) -c tree/tree.c

utils.o:  utils/utils.c
	$(CC) $(CFLAGS) -c utils/utils.c

stack.o:  stack/stack.c
	$(CC) $(CFLAGS) -c stack/stack.c


clean: 
	$(RM) mirr *.o executables/*