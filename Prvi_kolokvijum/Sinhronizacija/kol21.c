/*Korišćenjem programskog jezika C kreirati Linux program u kome glavna nit kreira celobrojni niz od N elemenata i
popunjava ga slučajnim vrednostima iz opsega [0, 10]. Glavna nit pokreće dve dodatne niti koje su zadužene za
sortiranje niza. Prva dodatna nit sortira prvu polovinu niza (0-N/2 – 1) u rastući redosled a druga nit sortira
drugu polovinu niza (N/2 – N-1) u opadajući redosled. Kada obe dodatne niti završe sa sortiranjem, glavna nit
štampa sortirani niz. Za sinhronizaciju niti koristiti mutex-e i uslovne promenljive.*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10

void* prvaPolovina(void* arg);
void* drugaPolovina(void* arg);

pthread_mutex_t mutex1;
// pthread_mutex_t mutex2;
pthread_cond_t cond1, cond2;

// int N;
int bafer[N];
int prvaGotova=0;
int drugaGotova=0;

int main()
{
    pthread_t nit1, nit2;

    pthread_mutex_init(&mutex1, NULL);
//     pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);

//     printf("Unesite N\n");
//     scanf("%d", N);

    printf("Niz u main-u:\n");
    for(int i=0; i<N; i++)
    {
        bafer[i]=rand() % 10;
        printf("%d\n", bafer[i]);
    }

    pthread_create(&nit1, NULL, (void*)prvaPolovina, (void*) NULL);
    pthread_create(&nit2, NULL, (void*)drugaPolovina, (void*) NULL);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    pthread_mutex_lock(&mutex1);

    while(!prvaGotova && !drugaGotova)
    {
        pthread_cond_wait(&cond1, &mutex1);
        pthread_cond_wait(&cond2, &mutex1);
    }

    printf("Niz posle sortiranja:\n");
    for(int i=0; i<N; i++)
    {
        printf("%d\n", bafer[i]);
    }

    pthread_mutex_unlock(&mutex1);

//     pthread_join(nit1, NULL);
//     pthread_join(nit2, NULL);

    pthread_mutex_destroy(&mutex1);
//     pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);
}

void* prvaPolovina(void* arg)
{
    int pom;
    pthread_mutex_lock(&mutex1);

    for(int i=0; i<N/2-1; i++)
    {
        for(int j=i+1; j<N/2; j++)
        {
            if(bafer[i]>bafer[j])
            {
                pom=bafer[i];
                bafer[i]=bafer[j];
                bafer[j]=pom;
            }
        }
    }
    prvaGotova=1;
    pthread_cond_signal(&cond1);

    pthread_mutex_unlock(&mutex1);
}

void* drugaPolovina(void* arg)
{
    int pom;
    pthread_mutex_lock(&mutex1);

    for(int i=N/2; i<N-1; i++)
    {
        for(int j=i+1; j<N; j++)
        {
            if(bafer[i]<bafer[j])
            {
                pom=bafer[i];
                bafer[i]=bafer[j];
                bafer[j]=pom;
            }
        }
    }
    drugaGotova=1;
    pthread_cond_signal(&cond2);

    pthread_mutex_unlock(&mutex1);

}
