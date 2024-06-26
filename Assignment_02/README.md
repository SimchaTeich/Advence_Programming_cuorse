# Using threads
The goal is to modify the [primeCounter.c](./Instructions/primeCounter.c) program that accepts a sequence of random numbers, and sums up the number of primes. The change should lead to the reduction of the running time, by threads.

Limitation: do not exceed 2MB of memory while running.

## The original
Go to the [Istructions](./Instructions/) folder and run the following commands:

```
make
```
```
time ./randomGenerator 10 10000000 | ./primeCounter
```
![](00%20-%20original%20counter%20results.png)

```
./randomGenerator 10 10000000 | valgrind --tool=massif --massif-out-file=massif.out ./primeCounter
```
```
ms_print massif.out
```
![](./01%20-%20original%20counter%20results.png)

In conclusion:
* time for 10M numbers: `0m43.341s`
* memory for 10M numbers: `4.516KB`


## Solution with original `isPrime` function
Go to the [with_original_isPrime](./mySolution/with_original_isPrime/) folder and run the following commands:

```
make
```
```
time ./randomGenerator 10 10000000 | ./myPrimeCounter
```
![](./02%20-%20my%20counter%20with%20original%20isPrime%20results.png)

```
./randomGenerator 10 10000000 | valgrind --tool=massif --massif-out-file=massif.out ./myPrimeCounter
```
```
ms_print massif.out
```

![](./03%20-%20my%20counter%20with%20original%20isPrime%20results.png)

In conclusion:
* time for 10M numbers: `0m10.203s`
* memory for 10M numbers: `6.266KB`

## Solutions with my `isPrime` function
Go to the [with_my_isPrime](./mySolution/with_my_isPrime/) folder and run the following commands:

```
make
```
```
time ./randomGenerator 10 10000000 | ./myPrimeCounter
```
![](./04%20-%20my%20counter%20with%20my%20isPrime%20results.png)

```
./randomGenerator 10 10000000 | valgrind --tool=massif --massif-out-file=massif.out ./myPrimeCounter
```
```
ms_print massif.out
```
![](./05%20-%20my%20counter%20with%20my%20isPrime%20results.png)

In conclusion:
* time for 10M numbers: `0m2.503s`
* memory for 10M numbers: `6.266KB`
