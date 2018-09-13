NAME = razryv

.PHONY: compile all clean clean_all debug

compile:
	gcc -o $(NAME) $(NAME).c -lm

all: compile
	./gen_results.sh

debug:
	gcc -o $(NAME) $(NAME).c -lm -g

clean:
	rm -f *.o
	rm -f $(NAME)

clean_all: clean
	rm -rf results
