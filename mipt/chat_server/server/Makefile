
.PHONY: all clean clean_all

all: client_interaction.o debug.o misc.o
	gcc -o server main.c client_interaction.o debug.o misc.o -pthread

client_interaction.o:
	gcc -o client_interaction.o -c client_interaction.c -pthread

debug.o:
	gcc -o debug.o -c debug.c -pthread

misc.o:
	gcc -o misc.o -c misc.c -pthread

clean:
	rm -f *.o
	rm -f server

clean_all: clean
	rm -f server_log chat_log passwd.db
