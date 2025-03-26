/*Korišćenjem programskog jezika C napisati Linux program koji kreira dodatni proces dete. Roditelj  generiše slučajne brojeve u opsegu od 0 do 999 i korišćenjem reda poruka šalje procesu detetu.  Brojevi se šalju sa pridruženim prioritetom pri čemu jednocifreni brojevi imaju prioritet 1, dvocifreni  prioritet 2, a trocifreni prioritet 3. Proces dete sumira primljene brojeve u tri različite sume (za svaki  prioritet zasebna suma) tako što sumiranje imaju brojevi prioriteta 1, zatim brojevi prioriteta 2 a tek  na kraju brojevi prioriteta 3. Proces dete sumiranje prekida kada zbir sve tri sume pređe vrednost  50000. O prekidu sumiranja proces dete korišćenjem istog reda poruka obaveštava proces roditelj  kako bi prekinuo generisanje brojeva*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define message_key 10001
#define max_message 1000

struct msgbuf
{
    long type;
    char text[max_message];
};

int main()
{
    int pid;
    int msqid;
    struct msgbuf bafer;
    int broj;
    char pom[max_message];
    char pomK[max_message]="";

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    pid=fork();

    if(pid==0)//dete
    {
        if(execl("2kol23a", "2kol23a", NULL)<0)
        {
            printf("Doslo je do greske prilikom pokretanja programa\n");
            exit(-1);
        }
    }

    do
    {
        broj=rand() % 1000;
        sprintf(pom, "%d", broj);

        strcpy(bafer.text, pom);

        if(broj<10)
            bafer.type=1;
        else if(broj<100)
            bafer.type=2;
        else if(broj<1000)
            bafer.type=3;

        printf("Poruka poslata %s\n", bafer.text);
        msgsnd(msqid, &bafer, max_message, 0);
//         printf("Poruka poslata %s\n", );

//         wait(NULL);

        msgrcv(msqid, &bafer, max_message, 5, 0);

        strcpy(pomK, bafer.text);

//         if(strcmp(pomK, "KRAJ")==0)
    }
    while(strcmp(pomK, "KRAJ")!=0);
    wait(NULL);

    msgctl(msqid, IPC_RMID, 0);
}
