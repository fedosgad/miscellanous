
.PHONY: all clean clean_all

all: tui.o debug.o
	gcc -o client main.c tui.o debug.o -pthread

tui.o:
	gcc -o tui.o -c tui.c

debug.o:
	gcc -o debug.o -c debug.c

clean:
	rm -f *.o
	rm -f client

clean_all: clean
	rm -f client_log
