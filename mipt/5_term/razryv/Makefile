NAME = razryv

.PHONY: compile all clean clean_all debug

compile: calculations.o
	gcc -o $(NAME) $(NAME).c calculations.o -lm

all: compile
	./gen_results.sh

debug:
	gcc -c -o calculations.o calculations.c -g
	gcc -o $(NAME) $(NAME).c calculations.o -lm -g

calculations.o: calculations.c
	gcc -c -o calculations.o calculations.c

clean:
	rm -f *.o
	rm -f $(NAME)

clean_all: clean
	rm -rf results
