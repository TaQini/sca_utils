FLAGS = -Wall -g -O3 -march=native -std=gnu11

all: L1 L2
	
L1: L1.c 
	gcc $(FLAGS) -o $@ $@.c -lpthread

L2: L2.c 
	gcc $(FLAGS) -o $@ $@.c -lpthread

%.o: %.c
	gcc $(FLAGS) -c $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm -f L1 L2
