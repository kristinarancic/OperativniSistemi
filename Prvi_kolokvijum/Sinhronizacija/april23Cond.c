/* Koriscenjem programskog jezika C napisati Linux program koij sadrzi bafer u koji moze da se semsti 10 integer brojeva. Jedna dodatna nit (worker thread) generise 5 slucajnih brojeva (u opsegu od 0 do 299) i upisuje ih na mesta sa parnim indeksima u ovaj bafer. Druga dodatna nit (worker thread) istovremeno generise 5 slucajnih brojeva(u opsegu od 300 do 499) i upisuje ih na mesta sa neparnim indeksima u ovaj bafer. Kada se u baferu nadje nova grupa brojeva (i na parnim i na neparnim indeksima), glavna nit (main funkcija) treba da odredi da li je zbir tih brojeva veci od 2000 i odstampa adekvatnu poruku n astandardnom izlazu. Ovaj postupak generisanja 10 brojeva i ispitivanja vrednosti njihovog zbira treba da se ponovi 5 puta. Adekvatno sinhronizovati rad niti koje generisu brojeve i glavne niti koja proverava njihov zbir.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* workerThread1(void* arg);
void* workerThread2(void* arg);

pthread_mutex_t mutex1, mutex2;
pthread_cond_t cond1, cond2;
int bafer[10];
int prviCond=0;
int drugiCond=0;

int main()
{
    pthread_t nit1, nit2;
    int suma=0;

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);

    pthread_create(&nit1, NULL, (void*)workerThread1, NULL);
    pthread_create(&nit2, NULL, (void*)workerThread2, NULL);

//     pthread_join(nit1, NULL);
//     pthread_join(nit1, NULL);

    for(int i=0; i<5; i++)
    {
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);

        while(!(prviCond && drugiCond))
        {
            pthread_cond_wait(&cond1, &mutex1);
            pthread_cond_wait(&cond2, &mutex2);
        }
        suma=0;
        printf("Ceo niz:\n");
        for(int j=0; j<10; j++)
        {
            suma+=bafer[j];
            printf("%d\n", bafer[j]);
        }
        if(suma<2000)
            printf("Suma je: %d i manja od 2000\n", suma);
        else
            printf("Suma je: %d i veca od 2000\n", suma);

        prviCond=0;
        drugiCond=0;

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);
    }

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);

    return 0;
}

void* workerThread1(void* arg)
{
    for(int i=0; i<5; i++)
    {
        pthread_mutex_lock(&mutex1);

//         printf("Brojevi na parnim indeksima:\n");
        for(int j=0;j<10; j+=2)
        {
            bafer[j] = rand() % 300;
            printf("Upisan broj na parnom indeksu %d\n", bafer[j]);
        }
        prviCond=1;
        pthread_cond_signal(&cond1);

        pthread_mutex_unlock(&mutex1);
    }
}

void* workerThread2(void* arg)
{
    for(int i=0; i<5; i++)
    {
        pthread_mutex_lock(&mutex2);

        for(int j=1;j<10; j+=2)
        {
            bafer[j] = rand() % 200 + 300;
            printf("Upisan broj na neparnom indeksu %d\n", bafer[j]);
        }
        drugiCond=1;
        pthread_cond_signal(&cond2);

        pthread_mutex_unlock(&mutex2);
    }
}
