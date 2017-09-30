all: sum_ret
	

sum_ret: using_return.c
	gcc -o sum_ret -lpthread using_return.c -pthread

clean:
	rm sum_ret

debug: using_return.c
	gcc -g -o sum_ret -lpthread using_return.c -pthread
