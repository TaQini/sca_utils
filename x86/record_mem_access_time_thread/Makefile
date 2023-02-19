FLAGS = -Wall -O3 -march=native -std=gnu11 -lpthread

all: L1 L2 LLC
	
L1: L1.c 
	gcc $(FLAGS) -o $@ $@.c -lpthread

L2: L2.c 
	gcc $(FLAGS) -o $@ $@.c -lpthread

LLC: LLC.c 
	gcc $(FLAGS) -o $@ $@.c -lpthread

%.o: %.c
	gcc $(FLAGS) -c $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm -f L1 L2 LLC
