1. What Linux platform do you use?

>> Ubuntu 14.04

2. How the preemptive scheduler is implemented.

>> For scheduling, whenever signal alarm is fired, a check is performed for the next runnable thread and context is switched. 
>> If looped on same thread that means there is no other runnable thread hence process would be exited.

3. How to compile your library and run your program.
>> Using the Makefile for compiling libraries and dining philosopher (dining_phil.c, dining_phil) implementation and executing the following command:

	make

>> For test cases, I have used the following command for compiling and running:

	gcc -Wall -pendantic -I. -o test_x text_x.c gtthread.a -g -w
	./text_x

4. How you prevent deadlocks in your Dining Philosophers solution.

>>  To prevent deadlock situation, of the 5 philosophers, while the second, third, fourth and fifth philosophers try to acquire left chopstick first and then the right chopstick, the first philosopher tries to acquire the right chopstick first and then the left chopstick. 
>>This avoids the situation when each philosopher will have one chopstick which would lead to deadlock.

5. Any thoughts you have on the project, including things that work especially well or which don't work.

Things that work:
>> The scheduler and mutex implementation has worked well to pass various test cases that I tried. 
>> I have tried to design the thread structure and manage the states in a simple way such that it's easy to debug and check various cases.

Things that don't work:
>> I tried using rand() function in dining philosopher implementation which led to deadlock situations certain random times of test. Hence, I have implemented a simple method for wait times to run a for loop and invoke gtthread_yield().
