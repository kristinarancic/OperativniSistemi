#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
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
    int msqid;
    struct msgbuf bafer;
    int broj;
    int suma1=0;
    int suma2=0;
    int suma3=0;
    int totalSuma=0;

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    while(totalSuma<=50000)
    {
        msgrcv(msqid, &bafer, max_message, 0, 0);
        printf("Poruka primljena %s\n", bafer.text);

        broj=atoi(bafer.text);

        if(bafer.type==1)
            suma1+=broj;
        else if(bafer.type==2)
            suma2+=broj;
        else if(bafer.type==3)
            suma3+=broj;

        totalSuma=suma1+suma2+suma3;

        printf("Suma: %d\n", totalSuma);

        if(totalSuma>50000)
        {
            strcpy(bafer.text, "KRAJ");
            bafer.type=5;
            msgsnd(msqid, &bafer, max_message, 0);
//             printf("Poruka poslata za KRAJ\n");
        }
        else
        {
            strcpy(bafer.text, "nije");
            bafer.type=5;
            msgsnd(msqid, &bafer, max_message, 0);
//             printf("Poruka poslata za KRAJ\n");
        }
    }

    msgctl(msqid, IPC_RMID, 0);
}
