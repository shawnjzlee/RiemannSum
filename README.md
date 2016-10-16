Riemann Sum
===

Project Overview
---
This project contains a integral calculator using riemann sum to learn, understand, and practice using the `std::thread` library. An `Rbarrier` class is implemented that allows work sharing to occur between threads after any one thread has completed its work.

It is a preliminary project for [**SpiceC - Programming Multicores for Parallelism**](http://www.cs.ucr.edu/~gupta/research/Projects/multi1.html) at [UC Riverside](http://ucr.edu) under the mentorship of [Keval Vora](http://www.cs.ucr.edu/~kvora001/) and Zachary Benavides.

## RBarrier
`RBarrier` is a class that allows threads that have completed its work to steal work from another thread, which increases the performance of the program. 

Currently, work sharing occurs if the candidate thread has less than half of its work completed. If the condition is true, a thread that has finished its work will begin work on this candidate thread.

---
The integral calculator using riemann sum takes in the 3 parameters through an input file (`left bound`, `right bound`, `number of parts`).

##### Basic algorithm:

1. Read from the input file the left and right bound of the equation to compute, as well as the number of parts (accuracy).

2. Read from the input file the number of parts (accuracy).

3. Distribute the parts equally to each thread (`num_threads` passed in as a command line argument)

4. Compute the local sum (the work done in each thread).

5. If a thread completes its work, use `r_barrier_wait()` to check if work sharing is available.

6. Steal work if condition is met, otherwise wait at the `pthread_barrier_wait()`.

7. After all work is completed, compute the global sum, and print the solution to the console.

### Make
In `/src` directory:

- `riemann_sum` requires g++ 5.x and c++14 to compile and run.
- `make` will compile the program with only the necessary flags `-pthread -lpthread -c -std=c++14`.
- `make debug` will compile the program with the listed flags above and `-debug -g` for debugging with, e.g., `gdb`.
- `make copy` will only copy the Makefile and `input.txt` in the case where those files have changed, but the source code has not.
- `make clean` will remove `./bin`.

In `/bin` directory:

- `make check ARGS="n"` will run the program once with `n` number of threads.
- `make check-multiplier ARGS1="n" ARGS2="m"` will run the program once with `n` number of threads and `m` multiplier (see below for multiplier explanation).
- `make check-nosharing ARGS="n"` will run the program once with `n` number of threads and disable work sharing between threads.
- `make check_d ARGS="n"` will pass the argument into `gdb` and run the program once with `n` number of threads.
- `make check_r ARGS="n"` will run the program with 1 to `n` number of threads.
- `make check_r-multiplier ARGS1="n" ARGS2="m"` will run the program with 1 to `n` number of threads and `m` multiplier.
- `make check_r-multiplier-nosharing ARGS1="n" ARGS2="m"` will run the program with 1 to `n` number of threads, `m` multiplier, and disable work sharing between threads.

#### `-multiplier` flag

This flag modifies the distribution algorithm so that the first thread will have `m` times more work than the other threads, where `m` is an argument passed into the program. This forces work sharing to occur between the threads when it is enabled.

##### Example
Program runs with 100 parts, 4 threads, 0 multiplier.

| Threads| Parts |
| ------ |:-----:|
| 1      | 25    |
| 2      | 25    |
| 3      | 25    |
| 4      | 25    |

Program runs with 100 parts, 4 threads, 6 multiplier.

| Threads| Parts |
| ------ |:-----:|
| 1      | 70    |
| 2      | 10    |
| 3      | 10    |
| 4      | 10    |