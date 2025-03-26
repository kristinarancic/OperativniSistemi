/*Koriscenjem programskog jezika C napisati Linux program koji kreira dodatni proces dete. ova dva procesa komuniciraju koriscenjem jednog reda poruka. Prvi proces sa tastature cita cele visecifrene brojeve i salje ih drugom procesu koristeci red poruka. Drugi proces treba da sabere cifre primljenog broja i da ih odstampa na standardnom izlazu. Ovaj postupak se ponavlja 10 puta.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define message_key 10001
#define max_message 20

struct msgbuf
{
    long type;
    char text[max_message];
};

int main()
{
    int msqid;
    int pid;
    struct msgbuf bafer;
    int uneti;
    char pom[max_message];

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    pid=fork();

    if(pid==0)//dete
    {
        if(execl("jun22a", "jun22a", NULL)<0)
        {
            printf("Doslo je do greske prilikom pokretanja drugog programa\n");
            exit(-1);
        }
    }

    for(int i=0; i<10; i++)
    {
        printf("Unesite broj:\n");
        scanf("%d", &uneti);

        sprintf(pom, "%d", uneti);

        bafer.type=1;
        strcpy(bafer.text, pom);
        msgsnd(msqid, &bafer, max_message, 0);


    }

    wait(NULL);

    msgctl(msqid, IPC_RMID, 0);
}
