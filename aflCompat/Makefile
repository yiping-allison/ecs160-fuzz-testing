CC = afl-clang
CFLAGS = -g

default: Tweeter.exe

Tweeter.exe: maxTweeter.o
	$(CC) $(CFLAGS) -o Tweeter.exe maxTweeter.o

maxTweeter.o: maxTweeter.c
	$(CC) $(CFLAGS) -c maxTweeter.c

clean:
	$(RM) Tweeter.exe *.o *~ 

.PHONY: run
run:
	afl-fuzz -i in -o out -- ./Tweeter.exe @@
