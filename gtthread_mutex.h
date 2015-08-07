#ifndef var1
#define var1

#include "gtthread_sched.h"
#include "gtthread.h"

typedef struct mutex_node{
	int lock;
} gtthread_mutex_t;

int  gtthread_mutex_init(gtthread_mutex_t *mutex);
int  gtthread_mutex_lock(gtthread_mutex_t *mutex);
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex);

#endif
