output: pthreads-h3.o
	gcc -O pthreads-h3.o -o pthreads-h3

pthreads-h3.o: pthreads-h3.c
	gcc -c pthreads-h3.c -o pthreads-h3.o

clean:
	rm *.o *~ pthreads-h3 primeFinder
