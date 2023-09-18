#include <pthread.h>

typedef struct {
	volatile unsigned int lecteurs;
	volatile unsigned int ecrivains;
	volatile unsigned int ecrivains_attente;
	pthread_cond_t cond_lecteurs;
	pthread_cond_t cond_ecrivains;
	pthread_mutex_t mutex;

} m_rwlock_t;

int m_rwlock_init(m_rwlock_t *l){
	pthread_cond_t cond_lecteurs = PTHREAD_COND_INITIALIZER;
        pthread_cond_t cond_ecrivains = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	l->cond_lecteurs = cond_lecteurs;
	l->cond_ecrivains = cond_ecrivains;
	l->mutex = mutex;
	l->lecteurs = 0;
	l->ecrivains = 0;
	l->ecrivains_attente = 0;

	return 0;
}
int m_rlock_lock(m_rwlock_t *l){
	pthread_mutex_lock(&(l->mutex));
	if(l->ecrivains != 0){
		/* Je suis bloqué*/
		pthread_cond_wait(&(l->cond_lecteurs),&(l->mutex));
	}
	l->lecteurs ++;
	pthread_mutex_unlock(&(l->mutex));
	return 0;
}

int m_wlock_lock(m_rwlock_t *l){
	pthread_mutex_lock(&(l->mutex));
	if((l->ecrivains != 0) || (l->lecteurs != 0)){
		/* Je suis bloqué*/
		l->ecrivains_attente ++;
                pthread_cond_wait(&(l->cond_ecrivains),&(l->mutex));
	}
	l->ecrivains ++;
        pthread_mutex_unlock(&(l->mutex));
	return 0;
}

int m_rwlock_unlock(m_rwlock_t *l){
	pthread_mutex_lock(&(l->mutex));
	if(l->ecrivains == 0){
		l->lecteurs--;
	} else {
		l->ecrivains--;
	}
	if(l->ecrivains_attente > 0){
		l->ecrivains_attente --;
		pthread_cond_signal(&(l->cond_ecrivains));
	} else {
		pthread_cond_broadcast(&(l->cond_lecteurs));
	}
	pthread_mutex_unlock(&(l->mutex));
	return 0;
}

int main(){
	return 0;
}

