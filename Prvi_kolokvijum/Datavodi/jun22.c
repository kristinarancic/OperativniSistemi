/*Koriscenjem programskog jezika C napisati LInux program koji kreira dodatni proces dete. Proces roditelj sa tastature ucitava putanju do tekstualne datoteke i jednu kljucnu rec. Ova dva procesa komuniciraju koriscenjem cevi (pipe-ova) od procesa roditelja ka procesu detetu. Proces roditelj koriscenjem pipe-a procesu detetu salje tu putanju do tekstualne datoteke i kljucne rec. Proces dete otvara tu tekstualnu datoteku i trazi u kojim linijama se javlja kljucna rec. Proces dete na ekranu stampa brojeve linija u kojima se javlja kljucna rec*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/wait.h>

int main()
{
    int pid;
    int pd[2];
    char bafer[80];
    char putanja[50];
    char kljucna[20];

    if(pipe(pd)==-1)
    {
        printf("Greska prilikom otvaranja datavoda\n");
        exit(-1);
    }

    if(pid=fork()!=0)//roditelj
    {
        close(pd[0]);

        printf("Unesite putanju: \n");
        scanf("%s", putanja);

        strcpy(bafer, putanja);
        strcat(bafer, " ");

        printf("Unesite kljucnu rec: \n");
        scanf("%s", kljucna);
        strcat(bafer, kljucna);

//         printf("Bafer u roditelju: %s\n", bafer);

        write(pd[1], bafer, strlen(bafer)+1);

        close(pd[1]);

        wait(NULL);
    }
    else//dete
    {
        close(pd[1]);

        read(pd[0], bafer, 80);

//         printf("Bafer u detetu: %s\n", bafer);

        char putanjaDete[50];
        char kljucnaDete[20];


        int i = 0;
        while(bafer[i]!=' ')
        {
            putanjaDete[i]=bafer[i];
            i++;
        }
        i++;
        int j=0;
        while(bafer[i]!='\0')
        {
            kljucnaDete[j]=bafer[i];
            j++;
            i++;
        }
//         printf("Putanja u detetu: %s\n", putanjaDete);
//         printf("Kljucna u detetu: %s\n", kljucnaDete);

        FILE* f;
        f=fopen(putanjaDete, "r");

        int pom=1;
        char pomocna[80];

        while(fgets(pomocna, 80, f) != NULL)
        {
//             fgets(pomocna, 80, f);
//             printf("%d. pomocna(linija) iz fajla: %s\n", pom, pomocna);
            if(strstr(pomocna, kljucnaDete) != 0)
            {
                printf("Kljucna rec %s se javlja u liniji %d\n", kljucnaDete, pom);

            }
            pom++;
        }

        close(pd[0]);
    }
}
