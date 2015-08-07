//Dining Philosopher's problem

#include "gtthread.h"
#include <stdlib.h>
#include<pthread.h>
#include <time.h>
gtthread_mutex_t stick_mutex;
#define CNT 5
int stick[CNT];

void* philosopher(void* arg)
{
	time_t t;
	srand((unsigned) time(&t));
	int x = (int) arg;
	int i;
	int eat_cnt = 0;
	int time;
	
	for(eat_cnt = 0; eat_cnt < 100; eat_cnt++){
		printf ("Philosopher %d hungry.\n", x);
		fflush(stdout);
		char first[] = "left";
		char second[] = "right";
		int sticks_phil[2] = {-1,-1};
		int lft = x - 1;
		int rght = x;
		if(x == CNT){
			rght = 0;
		}
		
		else if(x == 1){
			int tmp = lft;
			lft = rght;
			rght = tmp;
			strcpy(first, "right");
			strcpy(second, "left");

		}
		
		while(sticks_phil[0] == -1 || sticks_phil[1] == -1){
			gtthread_mutex_lock (&stick_mutex);
			if(stick[lft] == -1){
				printf ("Philosopher %d acquiring %s chopstick.\n", x, first);
				fflush(stdout);	
				sticks_phil[0] = lft;
				stick[lft] = x;
			}
			gtthread_mutex_unlock (&stick_mutex);

			gtthread_mutex_lock (&stick_mutex);
			if(stick[rght] == -1){
				printf ("Philosopher %d acquiring %s chopstick.\n", x, second);
				fflush(stdout);
				sticks_phil[1] = rght;
				stick[rght] = x;
			}
			gtthread_mutex_unlock (&stick_mutex);
		}

		long tmp = 100;
		if(sticks_phil[0] != -1 && sticks_phil[1] != -1){
			printf ("Philosopher %d eating.\n", x);
			fflush(stdout);
			for(time = 0; time <2; time++){
				gtthread_yield();
			}
				gtthread_mutex_lock (&stick_mutex);
				printf ("Philosopher %d releasing %s chopstick.\n", x, first);
				fflush(stdout);	
				stick[rght] = -1;
				printf ("Philosopher %d releasing %s chopstick.\n", x, second);
				fflush(stdout);
				stick[lft] = -1;
				gtthread_mutex_unlock (&stick_mutex);

		}
		
		printf ("Philosopher %d thinking. \n", x);
		fflush(stdout);
		for(time = 0; time <1; time++){
			gtthread_yield();
		}
		fflush(stdout);
	}
	printf("finished %d\n",x);
	
}

int main()
{
	gtthread_init(1000);
	int i;
	for(i = 0; i<CNT;i++)
	{
		stick[i] = -1;
	}
	gtthread_mutex_init(&stick_mutex);

	gtthread_t phil;
	for(i = 0; i <CNT;i ++){
		gtthread_create(&phil,philosopher, (void*) (i + 1));
	}
	for(i = 0; i <CNT;i ++){	
		gtthread_join(phil, NULL);
	}
	return 0;
}
