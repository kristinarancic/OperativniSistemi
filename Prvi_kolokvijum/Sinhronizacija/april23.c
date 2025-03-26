/* Koriscenjem programskog jezika C napisati Linux program koij sadrzi bafer u koji moze da se semsti 10 integer brojeva. Jedna dodatna nit (worker thread) generise 5 slucajnih brojeva (u opsegu od 0 do 299) i upisuje ih na mesta sa parnim indeksima u ovaj bafer. Druga dodatna nit (worker thread) istovremeno generise 5 slucajnih brojeva(u opsegu od 300 do 499) i upisuje ih na mesta sa neparnim indeksima u ovaj bafer. Kada se u baferu nadje nova grupa brojeva (i na parnim i na neparnim indeksima), glavna nit (main funkcija) treba da odredi da li je zbir tih brojeva veci od 2000 i odstampa adekvatnu poruku n astandardnom izlazu. Ovaj postupak generisanja 10 brojeva i ispitivanja vrednosti njihovog zbira treba da se ponovi 5 puta. Adekvatno sinhronizovati rad niti koje generisu brojeve i glavne niti koja proverava njihov zbir.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* workerThread1(void* arg);
void* workerThread2(void* arg);

sem_t prvi, drugi, mutex1, mutex2;
int bafer[10];

int main()
{
    pthread_t nit1, nit2;

    sem_init(&prvi, 0, 1);
    sem_init(&drugi, 0, 1);
    sem_init(&mutex1, 0, 0);
    sem_init(&mutex2, 0, 0);

    pthread_create(&nit1, NULL, (void*)workerThread1, (void*)NULL);
    pthread_create(&nit2, NULL, (void*)workerThread2, (void*)NULL);

//     pthread_join(nit1, NULL);
//     pthread_join(nit2, NULL);

    for(int i=0; i<5; i++)
    {
        sem_wait(&mutex1);
        sem_wait(&mutex2);

        int suma=0;

        for(int j=0; j<10; j++)
        {
            suma+=bafer[j];
        }
        printf("Suma: %d\n", suma);

        sem_post(&prvi);
        sem_post(&drugi);
    }

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    sem_destroy(&prvi);
    sem_destroy(&drugi);
    sem_destroy(&mutex1);
    sem_destroy(&mutex2);

}
void* workerThread1(void* arg)
{
//     sem_wait(&mutex);
    for(int j=0; j<5; j++)
    {
        sem_wait(&prvi);

        for(int i=0; i<10; i+=2)
        {
            bafer[i]=rand() % 300;
            printf("Generisan parni indeks: %d\n", bafer[i]);
        }
        sem_post(&mutex1);
    }
//     sem_post(&drugi);
}

void* workerThread2(void* arg)
{
//     sem_wait(&mutex);
    for(int j=0; j<5; j++)
    {
        sem_wait(&drugi);

        for(int i=1; i<10; i+=2)
        {
            bafer[i]=rand() % 200 + 300;
            printf("Generisan neparni indeks: %d\n", bafer[i]);
        }
        sem_post(&mutex2);
    }
//     sem_post(&drugi);
}
