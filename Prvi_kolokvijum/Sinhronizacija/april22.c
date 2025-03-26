/*Koriscenjem programskog jezika C napisati Linux program koji prvo startuje drugu nit, a zatim u glavnoj niti od korisnika ocekuje da unese ceo broj. Dugoj niti prosledjuje uneti broj, a druga nit odbrojava (i stampa na ekranu brojeve kako odbrojava) od 0 do unetog broja sa pauzom od 3 sekunde izmedju dva uzastopna broja. Zatim u glavnoj niti korisnik ponovo unosi ceo broj, prosledjuje ga drugoj niti koja odbrojava i ovaj proces odbrojavanja i unosa broja se ponavlja sve dok korisnik ne unese broj 99.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;
int broj;

void* funkcija(void* arg)
{
    while(1)
    {
//         printf("Nit1\n");
        pthread_mutex_lock(&mutex);
//         printf("Nit2\n");
//         printf("Broj u niti: %d\n", broj);
        for(int i=0; i<=broj;i++)
        {
            printf("%d\n", i);
            sleep(3);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t nit;
//     int broj;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&nit, NULL, (void*)funkcija, NULL);
    while(1)
    {
        pthread_mutex_lock(&mutex);
        printf("Unesite broj.\n");
        scanf("%d", &broj);
        pthread_mutex_unlock(&mutex);
        if(broj==99)
            exit(1);
    }
    pthread_join(nit, NULL);

    pthread_mutex_destroy(&mutex);
}
