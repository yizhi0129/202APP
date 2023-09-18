#include <pthread.h>

typedef struct{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned val;
	volatile unsigned current;
} bar_t;

int barrier_wait(bar_t* bar){
	pthread_mutex_lock(&(bar->mutex));
	bar->current ++;
	if(bar->current == bar->val){
		bar->current = 0;
		pthread_cond_broadcast(&(bar->cond));
	} else {
		pthread_cond_wait(&(bar->cond),&(bar->mutex));
	}
	pthread_mutex_unlock(&(bar->mutex));
	return 0;
}

int barrier_init(bar_t* bar, unsigned val){
	pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	bar->val = val;
	bar->current = 0;
	bar->mutex = mut;
	bar->cond = cond;
	return 0;
}

int main(){
	return 0;
}

