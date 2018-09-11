NAME = razryv1

.PHONY: all clean clean_all

all:
	gcc -o $(NAME) $(NAME).c -lm
	./gen_results.sh

clean:
	rm -f *.o
	rm -f $(NAME)

clean_all: clean
	rm -r results
