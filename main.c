#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t conditionEtat = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void* threadVert (void* arg);
void* threadOrange (void* arg);
void* threadRouge (void* arg);
void* threadEtat (void* arg);

enum couleur { VERT, ORANGE, ROUGE };

struct Feu
{
	enum couleur coul;
} Feu;


int main (void)
{
	pthread_t vert;
	pthread_t orange;
	pthread_t rouge;
	pthread_t etat;

	struct Feu feu;
	feu.coul = VERT;

	pthread_create (&etat, NULL, threadEtat, &feu); 
	pthread_create (&vert, NULL, threadVert, &feu);
	pthread_create (&orange, NULL, threadOrange, &feu); 
	pthread_create (&rouge, NULL, threadRouge, &feu);

	pthread_join (vert, NULL);
	pthread_join (orange, NULL);
	pthread_join (rouge, NULL);
	pthread_join (etat, NULL);

	return 0;
}


void* threadEtat(void* arg)
{
	struct Feu *feu = (struct Feu*)arg;
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait (&conditionEtat, &mutex);
		printf("le feu est : ");
		if(feu->coul == VERT){
			printf("VERT\n");
		}else if(feu->coul == ORANGE){
			printf("ORANGE\n");
		}else if(feu->coul == ROUGE){
			printf("ROUGE\n");
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void* threadVert(void* arg)
{
	struct Feu *feu = (struct Feu*)arg;
	while(1)
	{
		pthread_mutex_lock (&mutex);
		pthread_cond_signal (&conditionEtat);
		feu->coul = VERT;
		pthread_mutex_unlock (&mutex);
		sleep (3);
	}
	pthread_exit(NULL);
}

void* threadOrange(void* arg)
{
	sleep (1);
	struct Feu *feu = (struct Feu*)arg;
	while(1)
	{
		pthread_mutex_lock (&mutex);
		pthread_cond_signal (&conditionEtat);
		feu->coul = ORANGE;
		pthread_mutex_unlock (&mutex);
		sleep (3);
	}
	pthread_exit(NULL);
}

void* threadRouge(void* arg)
{
	sleep (2);
	struct Feu *feu = (struct Feu*)arg;
	while(1)
	{
		pthread_mutex_lock (&mutex);
		pthread_cond_signal (&conditionEtat);
		feu->coul = ROUGE;
		pthread_mutex_unlock (&mutex);
		sleep (3);
	}
	pthread_exit(NULL);
}