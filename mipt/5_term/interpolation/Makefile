NAME = interpolate

.PHONY: compile all clean debug

compile:
	gcc -o $(NAME) $(NAME).c

all: compile
	./gen_results.sh

debug:
	gcc -o $(NAME) $(NAME).c -g

clean:
	rm -f *.o
	rm -f $(NAME)
