/*Koriscenjem programskog jezika C napisati Linux program koji ucitava podatke iz tekstualne datoteke poruka.txt (red po red) i zatim Koriscenjem jednog reda poruka sve parne redove salje procesu koji konvertuje sva slova u velika, a sve neparne redove salje procesu koji konvertuje sva slova u mala. Drugi i treci proces konvertovane linije upisuje u datoteku sredjeno.txt */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
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

int main()
{
    FILE* f;
    int msqid;
    int pid1, pid2;
    struct msgbuf bafer;
    char procitano[max_message];
    int linija=1;

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    pid1=fork();

    if(pid1==0)//dete1
    {
        if(execl("sept23a", "sept23a", NULL)<0)
        {
            printf("Doslo je do greske prilikom otvaranja programa\n");
            exit(-1);
        }
    }

//     pid2=fork();
//     if(pid2==0)//dete2
//     {
//         if(execl("sept23a", "sept23a", NULL)<0)
//         {
//             printf("Doslo je do greske prilikom otvaranja programa\n");
//             exit(-1);
//         }
//     }

    f=fopen("poruka.txt", "r");

    while(fgets(procitano, max_message, f))
    {
        strcpy(bafer.text, procitano);
        if(linija % 2 != 0)
            bafer.type=1;
        else
            bafer.type=2;

        msgsnd(msqid, &bafer, max_message, 0);
//         printf("Poruka poslata\n");

        linija++;
    }
    strcpy(bafer.text, "END");
    bafer.type=1;
    msgsnd(msqid, &bafer, max_message, 0);
//     strcpy(bafer.text, "END");
    bafer.type=2;
    msgsnd(msqid, &bafer, max_message, 0);

    fclose(f);
    wait(NULL);
//     wait(NULL);

    msgctl(msqid, IPC_RMID, 0);
}
