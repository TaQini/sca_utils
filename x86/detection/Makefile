FLAGS = -Wall -O3 -march=native -std=gnu11

all: main
	
main: main.c cache.o
	gcc $(FLAGS) -o $@ cache.o $@.c 

%.o: %.c
	gcc $(FLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -f *.o main
