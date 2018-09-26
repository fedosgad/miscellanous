NAME = razryv

.PHONY: compile all clean clean_all debug

compile: calculations.o
	gcc -o $(NAME) $(NAME).c calculations.o -lm

all: compile
	./gen_results.sh

debug:
	gcc -o $(NAME) $(NAME).c -lm -g

calculations.o: calculations.c
	gcc -c -o calculations.o calculations.c

clean:
	rm -f *.o
	rm -f $(NAME)

clean_all: clean
	rm -rf results
