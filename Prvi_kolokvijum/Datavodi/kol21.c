/*Korišćenjem programskog jezika C kreirati Linux program koji kreira dodatni proces
 i ova dva procesa komuniciraju korišćenjem datavoda.
 Prvi process generiše dve slučajne celobrojne vrednosti u opsegu od 0 do 99 i
 ukoliko je zbir te dve generisane vrednosti veći od 100 prosleđuje ih drugom procesu
 korišćenjem datavoda. Nakon upisa drugi process proverava da li su dobijene vrednosti
 jednake i na ekranu štampa odgovarajuću poruku. Ovaj process generaisanja slučajnog para vrednosti,
 slanja drugom procesu i štampe poruke na ekranu se ponavlja 10 puta.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    int pid;
    int pd[2];
    int bafer[2];

    if(pipe(pd)==-1)
    {
        printf("Doslo je do greske prilikom kreiranja datavoda\n");
        exit(-1);
    }

    if(pid=fork()!=0)//roditelj
    {
        for(int j=0; j<10; j++)
        {
            int suma=0;
            close(pd[0]);
            printf("Bafer u roditelju\n");
            for(int i=0; i<2; i++)
            {
                bafer[i]=rand() % 100;
                printf("%d\n", bafer[i]);
                suma+=bafer[i];
            }
            if(suma>100)
            {
                write(pd[1], bafer, sizeof(int)*2);
                printf("Podaci su poslati\n");
            }
        }
        close(pd[1]);
        wait(NULL);
    }
    else//dete
    {
        close(pd[1]);
        while(read(pd[0], bafer, sizeof(int)*2)>0)
        {
            printf("Podaci su primljeni\n");
            if(bafer[0]==bafer[1])
                printf("Vrednosti %d i %d su jednake\n", bafer[0], bafer[1]);
            else
                printf("Vrednosti %d i %d nisu jednake\n", bafer[0], bafer[1]);
        }

        close(pd[0]);
    }
}
