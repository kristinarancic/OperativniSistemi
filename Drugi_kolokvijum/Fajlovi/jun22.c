/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu rekurzivno (maksimalno do dubine 3) brise sve regularne datoteke koje u imenu sadrze podstring "log" i vece su od 15kB. Za brisanje datoteka koristiti rm komandu Linux operativnog sistema.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void rekurzija(char* putanja, int level)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    char path[50];
    int pid;

    if((dp=opendir(putanja)) == NULL)
    {
        printf("Greska prilikom otvaranja direktorijuma\n");
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

        if(S_ISREG(statbuf.st_mode) && strstr(path, "log")!=0 && statbuf.st_size>15*1024)
        {
            pid=fork();
            if(pid==0)//dete
            {
                execlp("rm", "rm", path, NULL);
            }
            wait(NULL);
        }

        if(S_ISDIR(statbuf.st_mode) && level<2)
        {
            rekurzija(path, level+1);
//             level++;
        }
    }
}

int main(int argc, char* argv[])
{
    rekurzija(argv[1], 0);
}
