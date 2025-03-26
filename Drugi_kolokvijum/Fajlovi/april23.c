/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorrijumu (kao ulazni argument programa zadaje se apsolutna putanja do direktorijuma) i njegovim poddirektorijumima (mkasimalno do nivoa 5) odredjuje i stampa naziv najvece regularnee datoteke.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

long int maxSize=0;
char maxName[100];

void rekurzija(char* putanja, int level)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    char path[80];
    int levelPom;
    levelPom=level;

    if((dp=opendir(putanja))==NULL)
    {
        printf("Doslo je do greske prilikom otvaranja direktorijuma\n");
        exit(-1);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        if(strcmp(dirp->d_name, ".")==0 && strcmp(dirp->d_name, "..")==0)
            continue;

        strcpy(path, putanja);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);

        if(S_ISREG(statbuf.st_mode) && statbuf.st_size>maxSize)
        {
            maxSize=statbuf.st_size;
            strcpy(maxName, dirp->d_name);
        }

        if(S_ISDIR(statbuf.st_mode) && levelPom<5)
        {
            rekurzija(path, levelPom+1);
            levelPom++;
        }
    }
    closedir(dp);
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        printf("Nedovoljan broj argumenata\n");
        exit(-1);
    }

    rekurzija(argv[1], 0);

    printf("Najveca datoteka je %s, sa velicinom %ld\n", maxName, maxSize);
}
