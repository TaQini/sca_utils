FLAGS = -Wall -g -O3 -march=native -std=gnu11

all: a

a: a.c 
	gcc $(FLAGS) -o $@ $@.c 

%.o: %.c
	gcc $(FLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -f a 
