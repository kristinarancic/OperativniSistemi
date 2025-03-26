/*Koriscenjem prg jezika C napisati Linux program koji kreira dodatni proces sa kojim komunicira koriscenjem datavoda. Roditeljski proces generise 20 slucajnih brojeva u opsegu od 100 do 199 i salje ih putem datavoda procesu detetu. Proces dete n aekranu treba da odstampa samo brojeve koje dobije preko datavoda a koji su deljivi sa 3. U slucaju broja koji nije deljiv sa 3 ne stampa nista.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int bafer[20];

int main()
{
    int pid;
    int pd[2];

    if(pipe(pd)==-1)
    {
        printf("Doslo je do greske prilikom kreiranja datavoda\n");
        exit(-1);
    }
    if(pid=fork()!=0)//roditelj
    {
        close(pd[0]);
        printf("Bafer u roditelju:\n");
        for(int i=0; i<20; i++)
        {
            bafer[i]=rand() % 100 +100;
            printf("%d\n", bafer[i]);
        }
        write(pd[1], bafer, sizeof(int)*20);
        close(pd[1]);
        wait(NULL);
    }
    else//dete
    {
        close(pd[1]);

        read(pd[0], bafer, sizeof(int)*20);

        for(int i=0; i<20; i++)
        {
            if(bafer[i]%3==0)
                printf("Broj %d je deljiv sa 3\n", bafer[i]);
            else
                printf("Broj %d nije deljiv sa 3\n", bafer[i]);
        }

        close(pd[0]);
    }
}
