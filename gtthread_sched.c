#include "gtthread_sched.h"

extern struct gt_node* start;
extern struct gt_node* curr_thread;
extern struct gt_node* last_thread;
extern struct gt_node* free_thread;
int flag = 0;
extern int sched_period = 3000;
void scheduler(int sig_code)
{
	if(sig_code < 0){
		flag = 1;
	}
	else if(flag == 1){
		flag = 0;
		return;
	}
	struct gt_node* temp = curr_thread;
	if(curr_thread->state == 4)
	{
		if(curr_thread->prev){
			curr_thread->prev->next = curr_thread->next;
		}
		if(curr_thread->next){
			curr_thread->next->prev = curr_thread->prev;
			if(curr_thread == start){
				start = curr_thread->next;	
			}
			curr_thread = curr_thread->next;
		}
		else {
			if(curr_thread->id == start->id){
				exit(0);
			}
			curr_thread = start;
		}
		free(temp);
		temp = free_thread;
	}
	struct gt_node* temp1 = curr_thread;
	int cnt = 0;
	while(1){
		if(curr_thread->next){
			curr_thread = curr_thread->next;
			
		} else{
			curr_thread = start;
		}
		if((curr_thread->state == 1)||
				((curr_thread->joined == 1) && (curr_thread->state == 3 || curr_thread->slf_cancel == 1)))
		{
			break;
		}

		if(temp1->id == curr_thread->id){
			if(cnt >= 2){
				exit(0);
			} else {
				cnt ++;
			}
		}
	}
	if(temp == curr_thread)
	{
		return;
	}

	int return_val = swapcontext(&temp->context, &curr_thread->context);
	if(return_val != 0)
	{
		printf("Error %d\n",return_val);
	}
	return;
}

void set_timer()
{
    struct itimerval timer_val;
    struct sigaction sig_act;

    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sig_act.sa_handler = scheduler;
    if (sigaction(SIGVTALRM, &sig_act, NULL) == -1) {
        exit(0);
    }

    timer_val.it_value.tv_sec = 0;
    timer_val.it_value.tv_usec = sched_period;
    timer_val.it_interval.tv_sec = 0;
    timer_val.it_interval.tv_usec = sched_period;
    setitimer(ITIMER_VIRTUAL, &timer_val, NULL);
}
