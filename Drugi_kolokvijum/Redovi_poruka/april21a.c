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

int main()
{
    int msqid;
    struct msgbuf bafer;
    char linija[max_message];
    int brVelika=0;
    int brLinije=1;
    char pom[10];


    msqid=msgget(message_key, 0666 | IPC_CREAT);

    while(1)
    {
        brVelika=0;
        msgrcv(msqid, &bafer, max_message, 0, 0);
        printf("Poruka primljena\n");
        strcpy(linija, bafer.text);

        if(strcmp(linija, "END")==0)
        {
            break;
        }
        for(int i=0; i<strlen(linija)+1; i++)
        {
            if(linija[i]>='A' && linija[i]<='Z')
            {
                brVelika++;
            }
        }

        bafer.type=brLinije;
        sprintf(pom, "%d", brVelika);
        strcpy(bafer.text, pom);

        msgsnd(msqid, &bafer, max_message, 0);
        brLinije++;

    }
}
