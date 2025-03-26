/*Koriscenjem programskog jezika C kreirati Linux rogram koji kopira datoteku cije se ime prosledjuje kao prvi argument komandne linije tako sto kreira dodatni proces. Glavni proces iz izvorisne datoteke cita N bajtova (N se programu prosledjuje ka drugi argument komandne linije) i koriscenjem datovoda procitane bajtove prosledjuje drugom procesu koji te bajtove upisuje u odredisnu datoteku cije se ime prosledjuje kao treci argument komandne linije. Ova procedura se ponavlja dok se ne iskopira kompletna datoteka.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int pid;
    int pd[2];

    int N = atoi(argv[2])+1;
    char buff[N];

    if(argc<4)
    {
        printf("Nedovoljan broj argumenata\n");
        exit(-1);
    }

    if(pipe(pd) == -1)
    {
        printf("Doslo je do greske prilikom kreiranja datavoda\n");
        exit(-1);
    }
    if(pid=fork() != 0)//roditelj
    {
        FILE* f;
        f = fopen(argv[1], "r");
//         fgets(buff, N, f);

        close(pd[0]);//zatvaramo kraj za citanje

        while(!feof(f))
        {
            fgets(buff, N, f);
            printf("Roditelj: %s\n", buff);
            write(pd[1], buff, N);
        }

        fclose(f);
        close(pd[1]);
        wait(NULL);
    }
    else//dete
    {
        FILE* f2;
        f2=fopen(argv[3], "w");
        close(pd[1]);

        int bytesRead;
        while ((bytesRead = read(pd[0], buff, N)) > 0)
        {
            //read(pd[0], buff, N);
            printf("Dete: %s\n", buff);
            fputs(buff, f2);
        }


        fclose(f2);

        close(pd[0]);
    }
}
