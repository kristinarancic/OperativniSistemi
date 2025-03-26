/*Koriscenjem programskog jezika C kreirati Linux program koji kao argument komandne linije prihvata imena dve tekstualne datoteke. Program kreira dve niti. Prva nit otvara datoteku cije je ime prvi argument komadne linije i cita liniju po liniju. Druga nit na isti nacin otvara i cita liniju po liniju iz datoteke cije je ime drugi argument komandne linije. Ove dve niti naizmenicno upisuju procitane linije u zajednicku izlaznu datoteku pod imenom zbir.txt tako da su neparne linije u zbirnoj datoteci iz prve ulazne datoteke, a parne linije iz druge ulazne daoteke. Svaka nit ispred linije teksta u datoteku treba da upise ime izvorisne datoteke i broj linije iz izvorisne datoteke sa koje je procitana ta linija teksta*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>

void* prva(void* arg);
void* druga(void* arg);

sem_t sem1, sem2;

FILE* fz;

int main(int argc, char* argv[])
{
    pthread_t nit1, nit2;

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

    fz=fopen("zbir.txt", "w");

    pthread_create(&nit1, NULL, (void*)prva, (void*)argv[1]);
    pthread_create(&nit2, NULL, (void*)druga, (void*)argv[2]);
//
    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    fclose(fz);
}

void* prva(void* arg)
{
    char* putanja = (char*)arg;
//     printf("%s\n", putanja);
    char linija[80];
    char pom[80];
    FILE* f;
    f=fopen(putanja, "r");

    int i=1;
    char* pomi;
    while(fgets(linija, 80, f))
    {
        sprintf(pomi, "%d", i);
//         fgets(linija, 80, f);
        strcpy(pom, putanja);
        strcat(pom, pomi);
        strcat(pom, linija);
        sem_wait(&sem1);
        fputs(pom, fz);
        sem_post(&sem2);
        i++;
    }
    fclose(f);
}

void* druga(void* arg)
{
    char* putanja = (char*)arg;
//     printf("%s\n", putanja);
    char linija[80];
    char pom[80];
    FILE* f;
    f=fopen(putanja, "r");

    int i=1;
    char* pomi;
    while(fgets(linija, 80, f))
    {
        sprintf(pomi, "%d", i);
//         fgets(linija, 80, f);
        strcpy(pom, putanja);
        strcat(pom, pomi);
        strcat(pom, linija);
        sem_wait(&sem2);
        fputs(pom, fz);
        sem_post(&sem1);
        i++;
    }
    fclose(f);
}
