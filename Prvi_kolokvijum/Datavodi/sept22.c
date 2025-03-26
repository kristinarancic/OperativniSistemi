/*Koriscenjem programskog jezika C napisati Linux program koji kreira dodatni proces sa kojim komunicira korscenjem datavoda. Roditeljski proces generise 10 slucajnih brojeva u opsegu od 0 do 99 i salje ih putem datavoda procesu detetu. Proces dete na ekran treba da odstampa samo brojeve koje dobije preko datavoda a koji su neparni. U slucaju broja koji je paranstampa poruku "PARAN BROJ"*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pid;
    int pd[2];
    int bafer[10];

    if(pipe(pd) == -1)
    {
        printf("Doslo je do greske prilikom otvaranja datavoda\n");
        exit(-1);
    }

    if(pid=fork() != 0)//roditelj
    {
        close(pd[0]);
        printf("Bafer u roditelju:\n");
        for(int i=0;i<10;i++)
        {
            bafer[i]=rand() % 100;
            printf("%d\n", bafer[i]);
        }

        write(pd[1], bafer, sizeof(int)*10);

        close(pd[1]);
        wait(NULL);
    }
    else//dete
    {
        close(pd[1]);

        read(pd[0], bafer, sizeof(int)*10);

        for(int i=0; i<10; i++)
        {
            if(bafer[i] % 2 ==0)
                printf("Broj %d je paran.\n", bafer[i]);
            else
                printf("Broj %d je neparan.\n", bafer[i]);
        }

        close(pd[0]);
    }
}
