/*Koriscenjem programskog jezika C napisati Linux program koji u datoteku izlaz.txt upisuje niz od prvih 20 parnih i 20 neparnih brojeva redom. Parne brojeve upisuje jedna nit, a neparne druga nit. Sinhronizacijom niti semaforima obezbediti da se u datoteku parni i neparni brojevi upisuju naizmenicno.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

void* parni(void* arg);
void* neparni(void* arg);

sem_t par, nepar;// mutex;
int bafer[40];

int main()
{
    pthread_t nit1, nit2;
    FILE* f;

    sem_init(&par, 0, 0);
    sem_init(&nepar, 0, 1);

    f=fopen("izlaz.txt", "w");

    pthread_create(&nit2, NULL, (void*)neparni, (void*)f);
    pthread_create(&nit1, NULL, (void*)parni, (void*)f);

    pthread_join(nit2, NULL);
    pthread_join(nit1, NULL);

    sem_destroy(&par);
    sem_destroy(&nepar);

//     for(int i=0; i<40; i++)
//     {
//         printf("%d ", bafer[i]);
//     }
//     printf("\n");
}

void* parni(void* arg)
{
    FILE* fd = (FILE *)arg;
    char str[10];
    for(int i=1; i<40; i+=2)
    {
        sem_wait(&par);
//         bafer[i]=i+1;
        sprintf(str, "%d", i+1);
        strcat(str, " ");
        fputs(str, fd);
        sem_post(&nepar);
    }
}

void* neparni(void* arg)
{
    FILE* fd = (FILE *)arg;
    char str[10];
    for(int i=0; i<40; i+=2)
    {
        sem_wait(&nepar);
//         bafer[i]=i+1;
        sprintf(str, "%d", i+1);
        strcat(str, " ");
        fputs(str, fd);
        sem_post(&par);
    }
}
