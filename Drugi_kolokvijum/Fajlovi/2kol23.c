/*Korišćenjem programskog jezika C napisati Linux program koji za tekućei direktorijum generiše  statistiku tako što prebrojava: podfoldere, linkove, prazne regularne datoteke i regularne datoteke čija  je veličina veća od 0B. Na kraju program na ekranu štampa ovu statistiku (brojeve stavki po tipu)*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

void rekurzija(char* putanja)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    char path[80];
    int podFolderi=0;
    int linkovi=0;
    int prazne=0;
    int regularne=0;

    if((dp=opendir(putanja))==NULL)
    {
        printf("Doslo je do greske prilikom otvaranja direktorijuma\n");
        exit(-1);
    }

    while((dirp=readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0)
            continue;

        strcpy(path, putanja);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);

        if(S_ISDIR(statbuf.st_mode))
            podFolderi++;
        else if(S_ISLNK(statbuf.st_mode))
            linkovi++;
        else if(S_ISREG(statbuf.st_mode) && statbuf.st_size==0)
            prazne++;
        else if(S_ISREG(statbuf.st_mode) && statbuf.st_size>0)
            regularne++;

    }
    printf("Direktorijum ima %d podFoldera, %d linkova, %d praznih datoteka i %d regularnih\n", podFolderi, linkovi, prazne, regularne);

    closedir(dp);
}

int main()
{

    rekurzija(".");

    return 0;
}
