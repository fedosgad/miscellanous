all: compile
	

compile: tasks.c
	gcc -o tasks -lpthread tasks.c -pthread

clean:
	rm tasks

debug: tasks.c
	gcc -g -o tasks -lpthread tasks.c -pthread
