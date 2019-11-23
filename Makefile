CC = gcc
CFLAGS = -Wall -Werror

default: maxTweeter.exe

maxTweeter.exe: maxTweeter.o
	$(CC) $(CFLAGS) -o maxTweeter.exe maxTweeter.o

maxTweeter.o: maxTweeter.c
	$(CC) $(CFLAGS) -c maxTweeter.c

clean:
	$(RM) maxTweeter.exe *.o *~ 