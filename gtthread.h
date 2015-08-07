#ifndef var
#define var

#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h> 
#include <unistd.h>
#include <assert.h>
#include "gtthread_mutex.h"
#include "gtthread_sched.h"

// 0-Empty,1-Runnable,2-Wait,3-Done,4-Exit,5-cancel
typedef int gtthread_t;

struct gt_node{
	gtthread_t id;
        void **return_val;
        ucontext_t context;
        int state;
	int joined;
	int slf_cancel;
	struct gt_node* dep;	
	struct gt_node* next;
	struct gt_node* prev;
};

void gtthread_init(long period);
int  gtthread_create(gtthread_t *thread,
                     void *(*start_routine)(void *),
                     void *arg);
int  gtthread_join(gtthread_t thread, void **status);
void gtthread_exit(void *retval);
int  gtthread_yield(void);
int  gtthread_equal(gtthread_t t1, gtthread_t t2);
int  gtthread_cancel(gtthread_t thread);
gtthread_t gtthread_self(void);

#endif
