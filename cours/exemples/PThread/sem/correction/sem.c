#include <pthread.h>
#include <stdio.h>
#include <assert.h>

typedef struct{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	volatile unsigned int value;
} m_semaphore_t;

int m_sem_init(m_semaphore_t* sem, unsigned int val){
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
	sem->mutex = mut; 
	sem->cond = cond;
	sem->value = val;
	return 0;
}

int m_sem_wait(m_semaphore_t* sem){
	pthread_mutex_lock(&(sem->mutex));
	if(sem->value == 0){
		pthread_cond_wait(&(sem->cond),&(sem->mutex));
	}
	assert(sem->value > 0);
	sem->value --;
	pthread_mutex_unlock(&(sem->mutex));
}

int m_sem_post(m_semaphore_t *sem){
	pthread_mutex_lock(&(sem->mutex));
	sem->value ++;
	pthread_cond_signal(&(sem->cond));
	pthread_mutex_unlock(&(sem->mutex));
}


int main(){
	return 0;
}

