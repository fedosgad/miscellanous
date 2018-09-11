NAME = razryv1

.PHONY: compile all clean clean_all

compile:
	gcc -o $(NAME) $(NAME).c -lm

all: compile
	./gen_results.sh

clean:
	rm -f *.o
	rm -f $(NAME)

clean_all: clean
	rm -rf results
