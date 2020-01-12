.PHONY: all plot test clean

CC=gcc
CFLAGS=-std=gnu99 -Wall -pedantic -Werror

all: test_ring filter

ring.o: ring.c ring.h
	$(CC) $(CFLAGS) -c ring.c

wavfile.o: wavfile.c wavfile.h
	$(CC) $(CFLAGS) -c wavfile.c

test_ring: ring.o test_ring.c
	$(CC) $(CFLAGS) -o test_ring test_ring.c ring.o

filter: ring.o wavfile.o filter.c
	$(CC) $(CFLAGS) filter.c -o filter -lm wavfile.o ring.o

test: test_ring
	./test_ring

plot:
	gnuplot -p -c plot_clean
	gnuplot -p -c plot_noisy
	gnuplot -p -c plot_filtered

clean:
	rm -f *.png
	rm -f *.wav
	rm -f *.csv
	rm -f *.o
	rm -f test_ring
	rm -f filter
