/*Koriscenjem programskog jezika C napisati Linux program koji kreira dodatnu radnu nit, a zatim u glavnoj niti od korisnika ocekuje da unese ceo broj. Glavna nit prosledjuje drugoj (radnoj) niti uneti broj, a druga nit odbrojava unazad (i stampa na ekranu brojeve kako odbrojava) od unetog broja do 0 sa pauzom od 3 sekunde izmdju dva uzastopna broja. Zatim u glavnoj niti korisnik ponovo unosi ceo broj i ovaj proces odrobjavanja i unosa broja se ponavlja sve dok korisnik ne unese "KRAJ". Sinhronizaciju rada niti obezbediti semaforima.*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

void* funkcija(void* arg);

sem_t radni, mains;
int uneti;

int main()
{
    pthread_t nit;
    char* broj;
//     int uneti;

    sem_init(&radni, 0, 0);
    sem_init(&mains, 0, 1);

    pthread_create(&nit, NULL, (void*)funkcija, NULL);

    while(1)
    {
        sem_wait(&mains);
        printf("Unesite broj ili KRAJ\n");
        scanf("%s", broj);

        if(strstr(broj, "KRAJ") != NULL)
        {
            exit(1);
        }

        uneti=atoi(broj);

//         pthread_create(&nit, NULL, (void*)funkcija, (void*) &uneti);

//         pthread_join(nit, NULL);

        sem_post(&radni);
    }
    pthread_join(nit, NULL);

    sem_destroy(&radni);
    sem_destroy(&mains);
}

void* funkcija(void* arg)
{
//     int broj = *((int *) arg);
//     int broj = uneti;

//     printf("Preneti broj u radnu: %d\n", uneti);

    while(1)
    {
        sem_wait(&radni);
        printf("Preneti broj u radnu: %d\n", uneti);
        for(int i=uneti; i>= 0; i--)
        {
            printf("%d\n", i);
            sleep(3);
        }
        sem_post(&mains);
    }
}
