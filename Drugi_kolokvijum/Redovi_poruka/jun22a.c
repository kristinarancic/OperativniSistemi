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
    struct msgbuf bafer;
    int broj;
//     char pom[max_message];
    int suma;
    int cifra;

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    for(int i=0; i<10; i++)
    {
        msgrcv(msqid, &bafer, max_message, 0, 0);

        suma=0;
        broj=atoi(bafer.text);
        printf("Preneti broj: %d\n", broj);

        while(broj>0)
        {
            cifra=broj % 10;
            suma+=cifra;
            broj/=10;
        }
        printf("Suma cifara prenetog broja je %d\n", suma);
    }

    msgctl(msqid, IPC_RMID, 0);
}
