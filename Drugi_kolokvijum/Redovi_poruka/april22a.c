/*Koriscenjem programskog jezika C napisati Linux program koji simulira problem proizvodjac/potrosac koriscenjem redova poruka. Glavni program se deli u dva procesa. Prvi proces (proizvodjac) kreira N slucajnih pozitivnih celih brojeva i salje ih drugom procesu. N se odredjuje tokom izvrsenja, takodje kao slucajan pozitivan ceo broj. Po zavrsetku slanja, prvi proces salje -1 kao kod za kraj. Drugi proces (potrosac) preuzima poslate brojee iz poruka i stampa ih na standardnom izlazu. Nakon sto primi broj -1 proces stampa koliko je dobio parnih a koliko neparnih brojeva. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
    int parni=0;
    int neparni=0;
    int primljeni;

    msqid=msgget(message_key, 0666 | IPC_CREAT);


    do
    {
        msgrcv(msqid, &bafer, message_max, 0, 0);
        primljeni = atoi(bafer.text);

        if(strcmp(bafer.text, "-1")==0)
        {
            printf("Broj parnih brojeva: %d, broj neparnih brojeva: %d\n", parni, neparni);
//             exit(1);
            break;
        }

        printf("Primljeni broj: %d\n", primljeni);

        if(primljeni %2 == 0)
            parni++;
        else
            neparni++;
    }
    while(strcmp(bafer.text, "-1")!=0);
//     while(1)
//     {
//         msgrcv(msqid, &bafer, sizeof(int), 0, 0);
//
//         if(bafer.text==-1)
//         {
//             printf("Broj parnih brojeva: %d, broj neparnih brojeva: %d\n", parni, neparni);
//             exit(1);
//         }
//
//         printf("Primljeni broj: %d\n", bafer.text);
//
//         if(bafer.text %2 == 0)
//             parni++;
//         else
//             neparni++;
//     }

    msgctl(msqid, IPC_RMID, 0);
}
