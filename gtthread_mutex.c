#include "gtthread_mutex.h"

extern int flag;
int  gtthread_mutex_init(gtthread_mutex_t *mutex){
	mutex = (struct gtthread_mutex_t*) malloc(sizeof(struct gtthread_mutex_t*));
	mutex->lock = 0;
}
int  gtthread_mutex_lock(gtthread_mutex_t *mutex) {
	flag = 1;
	while(mutex->lock == 1) {
		flag = 0;
		scheduler(-111);
		flag = 1;
	}
	flag = 0;
	mutex->lock = 1;
}
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex){
	flag = 1;
	if(mutex->lock == 1){
		flag = 0;
		mutex->lock = 0;
	}
}
