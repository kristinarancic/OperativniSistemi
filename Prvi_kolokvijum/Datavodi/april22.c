/*Koriscenjem programskog jezika C kreirati Linux program koji kreira dodatni proces sa kojim komunicira dvosmerno Koriscenjem datavoda. Roditeljski proces generise 10 slucajnih brojeva u opsegu od 200 do 299 i salje ih putem datavoda procesu detetu. Proces dete roditeljskom procesu koriscenjem datavoda vraca samo brojeve deljive sa 3 i to uvecane za 25. Roditeljski proces tako primljene brojeve od procesa deteta treba da odstampa na ekranu*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pid;
    int pd1[2];
    int pd2[2];
    int bafer[10];

    if(pipe(pd1)==-1)
    {
        printf("Doslo je do greske prilikom kreiranja datavoda\n");
    }
    if(pipe(pd2)==-1)
    {
        printf("Doslo je do greske prilikom kreiranja datavoda\n");
    }
    if(pid=fork()!=0)//roditelj
    {
        close(pd1[0]);
        close(pd2[1]);

        for(int i=0; i<10; i++)
        {
            bafer[i] = rand() % 100 + 200;
            printf("Generisani broj u roditelju: %d\n", bafer[i]);
        }

        write(pd1[1], bafer, sizeof(int)*10);

        int byteReader;
        int i=0;
//         int pom=0;
        while(byteReader = read(pd2[0], &bafer[i], sizeof(int)))
        {
            printf("Broj stigao u roditelju: %d\n", bafer[i]);
            i++;
        }
//         while(byteReader = read(pd2[0], bafer, sizeof(int)*10))
//         {
//             printf("Broj stigao u roditelja: %d\n", bafer[pom]);
//             pom++;
//         }
//         for(int j=0; j<pom; j++)
//             printf("Broj stigao u roditelja: %d\n", bafer[j]);

        close(pd1[1]);
        close(pd2[0]);

        wait(NULL);
    }
    else//dete
    {
        close(pd1[1]);
        close(pd2[0]);

        read(pd1[0], bafer, sizeof(int)*10);

        for(int i=0; i<10; i++)
            printf("Niz stigao u dete: %d\n", bafer[i]);

        int pomocni[10];
        int brojac=0;

        for(int i=0; i<10; i++)
        {
            if(bafer[i] % 3 ==0)
            {
                pomocni[brojac] = bafer[i];
                brojac++;
            }
        }

        for(int i=0; i<brojac; i++)
        {
            bafer[i]=pomocni[i]+25;
            printf("Pomocni niz iz deteta: %d\n", pomocni[i]);
            write(pd2[1], &bafer[i], sizeof(int));
        }

//         write(pd2[1], bafer, sizeof(int)*brojac);

        close(pd1[0]);
        close(pd2[1]);
    }
}
