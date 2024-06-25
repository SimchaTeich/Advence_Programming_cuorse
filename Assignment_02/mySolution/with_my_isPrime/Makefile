.PHONY: all
all: generator myPrimeCounter

generator: generator.c
	gcc -o randomGenerator generator.c
	
myPrimeCounter: myPrimeCounter.c
	gcc -o myPrimeCounter myPrimeCounter.c -lpthread

.PHONY: clean
clean:
	-rm randomGenerator myPrimeCounter 2>/dev/null
