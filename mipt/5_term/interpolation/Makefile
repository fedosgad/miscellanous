NAME = interpolate

.PHONY: compile all clean debug

compile:
	g++ -o $(NAME) $(NAME).cpp

all: compile
	./gen_results.sh

debug:
	g++ -o $(NAME) $(NAME).cpp -g

clean:
	rm -f *.o
	rm -f $(NAME)
