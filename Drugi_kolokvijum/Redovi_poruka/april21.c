/*Koriscenjem programskog jezika C napisati Linux program koji kreira dodatni proces sa kojim komunicira dvosmerno koriscenjem jednog reda poruka. Proces roditelj iz datoteke cije je ime prosledjeno kao argument komandne linije cita liniju po liniju teksta i svaku liniju salje procesu detetu u posebnoj poruci. Proces dete u svakoj primljenoj liniji prebrojava broj pojavljivanja velikih slova abecede i procesu roditelju vraca kroz isti red poruka za svaku analiziranu liniju teksta kroz posebnu poruku redni broj linije i broj pojavljivanja velikih slova. Glavni program te podatke stampa na ekranu (redni broj linije i broj pojavljivanja velikih slova). Kada se analiziraju sve linije teksta iz datoteke glavni program na ekranu treba da odstampa "KRAJ".*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define message_key 10001
#define max_message 80

struct msgbuf
{
    long type;
    char text[max_message];
};

int main(int argc, char* argv[])
{
    int pid;
    int msqid;
    FILE* f;
    struct msgbuf bafer;
    char linija[max_message];
    int p=0;

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    pid=fork();
    if(pid<0)
    {
        printf("Doslo je do greske prilikom kreiranja deteta\n");
        exit(-1);
    }

    if(pid!=0)//roditelj
    {
        f=fopen(argv[1], "r");

        while(fgets(linija, max_message, f))
        {
//             fgets(linija, max_message, f);
            strcpy(bafer.text, linija);
            bafer.type=1;
            p++;

            printf("Pre msgsnd\n");
            msgsnd(msqid, &bafer, max_message, 0);
            printf("Poruka poslata\n");
        }
        strcpy(bafer.text, "END");
        msgsnd(msqid, &bafer, max_message, 0);

        fclose(f);

        wait(NULL);
    }
    else//dete
    {
        if(execl("./april21a", "april21a", NULL)<0)
        {
            printf("Problem prilikom otvaranja drugog prorama\n");
            exit(-1);
        }
    }

    for(int i=0; i<p; i++)
    {
        msgrcv(msqid, &bafer, max_message, 0, 0);

        printf("U liniji %ld ima %s velikih slova\n", bafer.type, bafer.text);
    }

    msgctl(msqid, IPC_RMID, 0);
}
