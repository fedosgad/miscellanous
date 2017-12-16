
.PHONY: all clean clean_all

all: client_interaction.o debug.o
	gcc -o server main.c client_interaction.o debug.o -pthread

client_interaction.o:
	gcc -o client_interaction.o -c client_interaction.c

debug.o:
	gcc -o debug.o -c debug.c

clean:
	rm -f *.o
	rm -f server

clean_all: clean
	rm -f server_log chat_log passwd.db
