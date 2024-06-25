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



/* 
 * calculates (a * b) % c taking into account that a * b might overflow 
 */
long long mulmod(long long a, long long b, long long mod)
{
    long long x = 0,y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {    
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}



/* 
 * modular exponentiation
 */
long long modulo(long long base, long long exponent, long long mod)
{
    long long x = 1;
    long long y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}



/*
 * Miller-Rabin Primality test, iteration signifies the accuracy
 */
int Miller(long long p,int iteration)
{
 
    int i;
    long long s;
    if (p < 2)
    {
        return 0;
    }
    if (p != 2 && p % 2==0)
    {
        return 0;
    }
     s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (i = 0; i < iteration; i++)
    {
        long long a = rand() % (p - 1) + 1, temp = s;
        long long mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return 0;
        }
    }
    return 1;
}



// Function to check if a number is prime
bool isPrime(int n)
{	
	return Miller(n, 5) ? true : false;
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
	
	for (i = 0; i < number_of_processors; ++i)
        pthread_create(&threads[i], NULL, partialPrimeCounter, NULL);

    for (i = 0; i < number_of_processors; ++i)
        pthread_join(threads[i], NULL);

    printf("%d total primes.\n", total_counter);
	
	free(threads);
    return 0;
}
