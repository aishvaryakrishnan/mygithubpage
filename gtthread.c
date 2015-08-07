#include "gtthread.h"
#include "gtthread_sched.h"

#define STACK_SIZE SIGSTKSZ

// 0-Empty,1-Runnable,2-Wait,3-Done,4-Exit,5-cancel
extern int sched_period;

struct gt_node* start;
struct gt_node* free_thread;
struct gt_node* curr_thread;
struct gt_node* last_thread;
gtthread_t tid;
void func(void *(*start_routine)(void *), void *arg);
void cleaner();
struct gt_node* getThread(gtthread_t thread);

void gtthread_init(long period)
{
	sched_period = period;
	set_timer();
	start = (struct gt_node *) malloc(sizeof(struct gt_node));
	memset(start,0,sizeof(struct gt_node));
	start->id = tid;
	start->dep = NULL;
        start->state = 1;
	start->next =  NULL;

	free_thread = (struct gt_node *) malloc(sizeof(struct gt_node));
	memset(free_thread,0,sizeof(struct gt_node));
	free_thread->id = tid;
	free_thread->dep = NULL;
        free_thread->state = 1;
	free_thread->next =  NULL;
	free_thread->prev = NULL;
	free_thread->context.uc_stack.ss_sp = malloc(STACK_SIZE);
        free_thread->context.uc_stack.ss_size = (STACK_SIZE);
        free_thread->context.uc_stack.ss_flags = 0;

	curr_thread = start;
	last_thread = start;
	last_thread->prev = NULL;
	getcontext(&start->context);
	start->context.uc_stack.ss_sp = malloc(STACK_SIZE);
        start->context.uc_stack.ss_size = (STACK_SIZE);
        start->context.uc_stack.ss_flags = 0;
}


int  gtthread_create(gtthread_t *thread, void *(*start_routine)(void *), void *arg) 
{
	*thread = ++tid;
	struct gt_node* new_node = (struct gt_node *)malloc(sizeof(struct gt_node));
	memset(new_node,0,sizeof(struct gt_node));
	new_node->id = tid;
	new_node->dep = NULL;
        new_node->state = 1;
	new_node->next =  NULL;
	new_node->prev = last_thread;
	new_node->joined = 0;
	new_node->slf_cancel = 0;
	
	getcontext(&new_node->context); 
	new_node->context.uc_stack.ss_sp = malloc(STACK_SIZE);
        new_node->context.uc_stack.ss_size = (STACK_SIZE);
        new_node->context.uc_stack.ss_flags = 0;
	new_node->state = 1;
	last_thread->next = new_node;
	last_thread = new_node;
	makecontext(&new_node->context, func, 2, start_routine, arg);
	return 0;
}

int  gtthread_join(gtthread_t thread, void **status)
{
	struct gt_node* join_t = getThread(thread);
	curr_thread->return_val = status;
	join_t->dep = curr_thread;
	join_t->joined = 1;
	if(join_t->state == 5){
		if(join_t->return_val == NULL){
			cleaner(((void *)(size_t) -1), join_t->id);
		} else {
			cleaner(join_t->return_val, join_t->id);
		}
	} else {
		curr_thread->state = 2;
		gtthread_yield();
	}
	return 0;
}

struct gt_node* getThread(gtthread_t thread){
	struct gt_node* temp = start;
	while(1) {
		if(temp->id == thread){
			break;
		}
		if(temp->next){
			temp = temp->next;
		} else {
			break;
		}
	}
	return temp;
} 

void gtthread_exit(void *retval)
{	
	curr_thread->return_val = retval;
	curr_thread->state = 3;
	scheduler(-1);
	cleaner(retval,curr_thread->id);	
	scheduler(-1);
}
int  gtthread_yield(void)
{
	scheduler(-1);
	return 0;
}
int  gtthread_equal(gtthread_t t1, gtthread_t t2)
{
	return (t1 == t2) ;
}
int  gtthread_cancel(gtthread_t thread)
{
	struct gt_node* cancel_t = getThread(thread);
	int state = cancel_t->state;
	cancel_t->state = 5;
	if(cancel_t == curr_thread){
		curr_thread->slf_cancel = 1;
		scheduler(-1);
		cleaner(((void *)(size_t)-1), cancel_t->id);
		scheduler(-1);
	}
	if(state == 3){
		return 0;
	}
	cancel_t->return_val = NULL;
	return 0;
} 

gtthread_t gtthread_self()
{  
	return curr_thread->id;
}

void func(void *(*start_routine)(void *), void *arg)
{
	void* return_val = start_routine(arg);
	curr_thread->return_val = return_val;
	curr_thread->state = 3;
	scheduler(-1);
	cleaner(return_val,curr_thread->id);
	scheduler(-1);
}

void cleaner(void* return_val,gtthread_t thread)
{
	struct gt_node* clean_t = getThread(thread);
	struct gt_node* x = clean_t->dep;
	if(x)
	{	
		if(x->return_val){
			*(x->return_val) = return_val;
		}
		if(x->state != 4){
			x->state = 1;
		}
		clean_t->dep = NULL;
	}
	
	clean_t->state = 4;
	return;
}
