# These are variables. We usually define CC, CCLINK, CFLAGS, and OBJS
CC=gcc
CCLINK=$(CC) # Read variable CC using $(CC)
CFLAGS=-g -Wall -std=c99
OBJS=ransom.o
EXEC=prog.exe
RM=rm -rf *.o *.exe

#First rule that will compile all - default when running the file.
all: $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC)

ransom.o: ransom.c
	$(CC) $(CFLAGS) -c ransom.c

#Clean operation to delete all automatic generated files.
clean:
	$(RM)