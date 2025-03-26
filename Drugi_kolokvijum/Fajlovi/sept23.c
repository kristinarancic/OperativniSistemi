/*Koriscenjem programskog jezika C napisati Linux program koji kao ulazne argumente prihvata apsolutnu putanju do nekog direktorijuma i parametre n i m. Program ima zadatak da u zadatom direktorijumu iz n prvih regularnih datoteka odstampa m linija sa pocetka datoteke (pretpostaviti da je maksimalna duzina linije u datoteci 100 karaktera). */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int brLinija, brDatoteka;
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    char path[50];
    int brojac;
    FILE* f;
    char linija[100];

    brLinija=atoi(argv[2]);
    brDatoteka=atoi(argv[3]);

    if((dp=opendir(argv[1]))<0)
    {
        printf("Gresak prilikom otvaranja direktorijuma\n");
        exit(-1);
    }

    while((dirp=readdir(dp))!=NULL  )
    {
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);
        printf("Datoteka %s\n", path);


            if(S_ISREG(statbuf.st_mode) && brojac<brDatoteka)
            {
                printf("Stampa se iz datoteke %s\n", path);
                f=fopen(path, "r");
                for(int i=0; i<brLinija; i++)
                {
                    fgets(linija, 100, f);
                    printf("%d. linija: %s\n", i+1, linija);
                }
                brojac++;
                fclose(f);
            }
    }

    closedir(dp);
}
