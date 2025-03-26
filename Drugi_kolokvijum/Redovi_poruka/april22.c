/*Koriscenjem programskog jezika C napisati Linux program koji simulira problem proizvodjac/potrosac koriscenjem redova poruka. Glavni program se deli u dva procesa. Prvi proces (proizvodjac) kreira N slucajnih pozitivnih celih brojeva i salje ih drugom procesu. N se odredjuje tokom izvrsenja, takodje kao slucajan pozitivan ceo broj. Po zavrsetku slanja, prvi proces salje -1 kao kod za kraj. Drugi proces (potrosac) preuzima poslate brojee iz poruka i stampa ih na standardnom izlazu. Nakon sto primi broj -1 proces stampa koliko je dobio parnih a koliko neparnih brojeva. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define message_key 10001
#define message_max 10

struct msgbuf
{
    long type;
    char text[message_max];
};

int main()
{
    int pid;
    int msqid;
    struct msgbuf bafer;
    int N;
    int random;

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    pid=fork();
    if(pid<0)
    {
        printf("Doslo je do greske prilikom forkovanja\n");
        exit(-1);
    }

    if(pid==0)//dete
    {
        if(execl("april22a", "april22a", NULL)<0)
        {
            printf("greska prilikom pokretanja drugog programa\n");
            exit(-1);
        }
    }
    else//roditelj
    {
        N = rand() % 20;
        bafer.type=1;
        for(int i=0; i<N; i++)
        {
            random=rand() % 100;
            sprintf(bafer.text, "%d", random);
            msgsnd(msqid, &bafer, message_max, 0);

        }
        strcpy(bafer.text, "-1");
//         bafer.text=-1;
        msgsnd(msqid, &bafer, message_max, 0);

        wait(NULL);
    }
    msgctl(msqid, IPC_RMID, 0);
}
