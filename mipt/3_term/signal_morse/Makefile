all: tr rec
	

tr: transmit.c
	gcc -o tr transmit.c
rec: recieve.c
	gcc -o rec recieve.c

clean:
	rm tr rec

debug: transmit.c recieve.c
	gcc -g -o tr transmit.c
	gcc -g -o rec recieve.c
