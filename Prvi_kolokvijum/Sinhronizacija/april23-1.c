/* Koriscenjem programskog jezika C napisati Linux program koij sadrzi bafer u koji moze da se semsti 10 integer brojeva. Jedna dodatna nit (worker thread) generise 5 slucajnih brojeva (u opsegu od 0 do 299) i upisuje ih na mesta sa parnim indeksima u ovaj bafer. Druga dodatna nit (worker thread) istovremeno generise 5 slucajnih brojeva(u opsegu od 300 do 499) i upisuje ih na mesta sa neparnim indeksima u ovaj bafer. Kada se u baferu nadje nova grupa brojeva (i na parnim i na neparnim indeksima), glavna nit (main funkcija) treba da odredi da li je zbir tih brojeva veci od 2000 i odstampa adekvatnu poruku n astandardnom izlazu. Ovaj postupak generisanja 10 brojeva i ispitivanja vrednosti njihovog zbira treba da se ponovi 5 puta. Adekvatno sinhronizovati rad niti koje generisu brojeve i glavne niti koja proverava njihov zbir.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 10

int bafer[N];

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void * parna(void *arg)
{

    for(int j=0; j<5; j++)
    {
        pthread_mutex_lock(&mutex1);
        for(int i=0;i<10; i+=2)
        {
            bafer[i]=rand() % 300;
            printf("Generiasni broj na indeksu %d je: %d\n", i, bafer[i]);
        }
        pthread_mutex_unlock(&mutex1);
    }
//     for(int i=0;i<10; i+=2)
//     {
//         bafer[i]=rand() % 300;
//         printf("Generiasni broj na indeksu %d je: %d\n", i, bafer[i]);
//     }
//     pthread_mutex_unlock(&mutex1);
}
void * neparna(void *arg)
{
    for(int j=0; j<5; j++)
    {
        pthread_mutex_lock(&mutex2);
        for(int i=1;i<10; i+=2)
        {
            bafer[i]=rand() % 200 + 300;
            printf("Generiasni broj na indeksu %d je: %d\n", i, bafer[i]);
        }
        pthread_mutex_unlock(&mutex2);
    }
//     pthread_mutex_lock(&mutex2);
//     for(int i=1;i<10; i+=2)
//     {
//         bafer[i]=rand() % 200 + 300;
//         printf("Generiasni broj na indeksu %d je: %d\n", i, bafer[i]);
//     }
//     pthread_mutex_unlock(&mutex2);
}

int main()
{
    pthread_t nit1, nit2;

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_create(&nit1, NULL, (void *) parna, (void *)NULL);
    pthread_create(&nit2, NULL, (void *) neparna, (void *)NULL);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);


//         pthread_create(&nit1, NULL, (void *) parna, (void *)NULL);
//         pthread_create(&nit2, NULL, (void *) neparna, (void *)NULL);
//
//         pthread_join(nit1, NULL);
//         pthread_join(nit2, NULL);

        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);

        int suma=0;

        for(int j=0; j<N; j++)
        {
            suma+=bafer[j];
            printf("Broj: %d\n", bafer[j]);
        }
        if(suma>3000)
            printf("Suma brojeva %d je veca od 3000\n", suma);
        else
            printf("Suma brojeva %d je manja od 3000\n", suma);

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);

//     pthread_join(nit1, NULL);
//     pthread_join(nit2, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
}
