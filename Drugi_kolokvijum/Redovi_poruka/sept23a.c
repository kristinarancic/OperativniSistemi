#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

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
    struct msgbuf bafer;
    char procitano[max_message];
//     strcpy(procitano, "");
    char konvertovana[max_message];
//     int tip;

    msqid=msgget(message_key, 0666 | IPC_CREAT);

    f=fopen("sredjeno.txt", "w");

    do
    {
//         tip = atoi(argv[1]);
        msgrcv(msqid, &bafer, max_message, 0, 0);
        strcpy(procitano, bafer.text);


        if(bafer.type==1)
        {
            for(int i=0; i<strlen(procitano)+1; i++)
            {
                konvertovana[i]=toupper(procitano[i]);
            }
        }
        else if(bafer.type==2)
        {
            for(int i=0; i<strlen(procitano)+1; i++)
            {
                konvertovana[i]=tolower(procitano[i]);
            }
        }
        if(strcmp("END", procitano)==0)
            continue;

        fputs(konvertovana, f);

    }
    while(strcmp("END", procitano)!=0);

    fclose(f);

    msgctl(msqid, IPC_RMID, 0);
}
