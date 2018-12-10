NAME = diff_eq

.PHONY: compile all clean debug

compile:
	gcc -o $(NAME) $(NAME).c -lm

all: compile

debug:
	gcc -o $(NAME) $(NAME).c -lm -g
clean:
	rm -f *.o
	rm -f $(NAME)
