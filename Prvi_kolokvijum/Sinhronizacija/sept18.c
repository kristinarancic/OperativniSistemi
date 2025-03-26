/*Koriscenjem programskog jezika C napisati Linux program koji sadrzi bafer u koji se mogu smestiti dve celobrojne vrednosti/ Zasebna nit periodicno generise dva slucajna broja (u opsegu od 100 d 199) i upisuje ih u ovaj bafer. Kada se u baferu nadje novi par brojeva, glavna nit (main funkcija) treba da odredi da li su ta dva broja jednaka i odstampa adekvatnu poruku na standardnom izlazu. Ovaj postupak generisanja para brojeva i ispitivanje njihove jednakosti treba da se ponovi 10 puta*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* funkcija(void* arg);

sem_t sem1, sem2;
int bafer[2];

int main()
{
    pthread_t nit;

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

    pthread_create(&nit, NULL, (void*)funkcija, NULL);

    for(int i=0; i<10; i++)
    {
        sem_wait(&sem2);
        if(bafer[0]==bafer[1])
            printf("Brojevi %d i %d su jednaki\n", bafer[0], bafer[1]);
        else
            printf("Brojevi %d i %d nisu jednaki\n", bafer[0], bafer[1]);
        sem_post(&sem1);

    }
    pthread_join(nit, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
}

void* funkcija(void* arg)
{
    for(int i=0; i<10; i++)
    {
        sem_wait(&sem1);
        printf("Bafer u niti:\n");
        for(int j=0; j<2; j++)
        {
            bafer[j]=rand() % 100 +100;
            printf("%d\n", bafer[j]);
        }
        sem_post(&sem2);
    }
}
