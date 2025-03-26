/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu (kao ulazni argument programa zadaje se apsolutna putanja do direktorijuma) i rekurzivno u njegovim poddirektorijumima odredjuje i stampa imena svih regularnih datoteka koje su vece od 100KB tako da je dobijeni spisak sortiran po velicini datoteke u rastucem redosledu */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

int velicine[20];
char* imena[20];
int i=0;

void rekurzija(char* putanja)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    char path[80];

    if((dp=opendir(putanja))==NULL)
    {
        printf("Doslo je do greske prilikom otvaranja direktorijuma\n");
        exit(-1);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0)
        {
            continue;
        }

        strcpy(path, putanja);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);

        if(S_ISREG(statbuf.st_mode) && statbuf.st_size>5*1024)
        {
            printf("Datoteka %s, sa velicinom %ld je veca od 1024B\n", dirp->d_name, statbuf.st_size);
            velicine[i]=statbuf.st_size;
//             strcpy(imena[i], dirp->d_name);
            imena[i]=strdup(dirp->d_name);
            i++;
        }
        if(S_ISDIR(statbuf.st_mode))
        {
            rekurzija(path);
        }

    }
    closedir(dp);
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        printf("Nedovoljan broj argumenata komandne linije\n");
        exit(-1);
    }

    rekurzija(argv[1]);

    int pom;
    char pom1[80];

    for(int j=0; j<i-1; j++)
    {
        for(int k=j+1; k<i; k++)
        {
            if(velicine[j] > velicine[k])
            {
                pom = velicine[j];
                strcpy(pom1, imena[j]);
                velicine[j] = velicine[k];
                strcpy(imena[j], imena[k]);
                velicine[k] = pom;
                strcpy(imena[k], pom1);
            }
        }
    }

    for(int j=0; j<i; j++)
    {
        printf("Ime datoteke: %s, velicina: %d\n", imena[j], velicine[j]);
        free(imena[j]);
    }
}
