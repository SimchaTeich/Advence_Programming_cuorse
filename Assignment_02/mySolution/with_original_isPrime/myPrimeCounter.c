/*******************************************************************************************************************
* Greate thanks to https://www.sanfoundry.com/c-program-implement-rabin-miller-primality-test-check-number-prime/  *
* about miller-rabin algo                                                                                          *
* and for https://stackoverflow.com/questions/4586405/how-to-get-the-number-of-cpus-in-linux-using-c               *
* about the code gives the number of processors                                                                    *
********************************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <time.h> 
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define CHUNK_SIZE 1000

int             total_counter;                             /* the counter        */
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;  /* the counter's lock */
pthread_mutex_t stream_lock  = PTHREAD_MUTEX_INITIALIZER;  /* stdin lock         */



// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}



// Function for one thread
void *partialPrimeCounter(void *p)
{
    int numbers[CHUNK_SIZE];
    int primes = 0;
    int i      = 0;
    int j      = 0;

    while (1)
    {
	    // get input numbers
	    pthread_mutex_lock(&stream_lock);
        for (i = 0; i < CHUNK_SIZE && scanf("%d", &numbers[i]) != EOF; ++i)
	        ;
	    pthread_mutex_unlock(&stream_lock);

	    // count primes
	    for (j = 0; j < i; ++j)
	    {
	        if (isPrime(numbers[j]))
		        ++primes;
	    }

	    // get out if input stream is over
	    if (i < CHUNK_SIZE - 1)
	        break;
    }

    // append result to the gloabl counter
    pthread_mutex_lock(&counter_lock);
    total_counter += primes;
    pthread_mutex_unlock(&counter_lock);

    return NULL;
}



int main()
{
	long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
	pthread_t *threads        = (pthread_t *)malloc(number_of_processors * sizeof(pthread_t));
	int i                     = 0;
	
    // create threads
	for (i = 0; i < number_of_processors; ++i)
        pthread_create(&threads[i], NULL, partialPrimeCounter, NULL);

    // wait for join all of them
    for (i = 0; i < number_of_processors; ++i)
        pthread_join(threads[i], NULL);

    // print results
    printf("%d total primes.\n", total_counter);
    
	free(threads);
    return 0;
}
