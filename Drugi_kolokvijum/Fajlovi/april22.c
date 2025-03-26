/*Koriscenjem programskog jezika C napisati Linux program koji uporedjuje sadrzaj dva direktorijuma. Putanje do direktorijuma koji se uporedjuju se zadaju kao ulazni argumenti programa. Program za svaku regularnu datoteku iz prvog direktorijuma proverava da li postoji odgovarajuca datoteka u drugom direktorijumu (uporedjuje ime i velicinu datoteke). Ukoliko datoteka postoji, stampa se ime datoteke. Napomena: Pretpostaviti da prvi direktorijum ne sadrzi poddirektorijume*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[])
{
    DIR* dp1;
    DIR* dp2;
    struct dirent* dirp1;
    struct dirent* dirp2;
    struct stat statbuf1;
    struct stat statbuf2;
    char path1[80];
    char path2[80];

    if(argc<3)
    {
        printf("Nedovoljno argumenata\n");
        exit(-1);
    }

    if((dp1=opendir(argv[1]))==NULL)
    {
        printf("Direktorijum nije uspesno otvoren\n");
        exit(-1);
    }
    if((dp2=opendir(argv[2]))==NULL)
    {
        printf("Direktorijum nije uspesno otvoren\n");
        exit(-1);
    }

    while((dirp1=readdir(dp1))!=NULL)
    {
        if(strcmp(dirp1->d_name, ".")==0 || strcmp(dirp1->d_name, "..")==0)
        {
            continue;
        }

        strcpy(path1, argv[1]);
        strcat(path1, "/");
        strcat(path1, dirp1->d_name);

        stat(path1, &statbuf1);

        while((dirp2=readdir(dp2))!=NULL)
        {
            if(strcmp(dirp2->d_name, ".")==0 || strcmp(dirp2->d_name, "..")==0)
            {
                continue;
            }

            strcpy(path2, argv[2]);
            strcat(path2, "/");
            strcat(path2, dirp2->d_name);

            stat(path2, &statbuf2);

            if(S_ISREG(statbuf2.st_mode) && statbuf1.st_size == statbuf2.st_size && strcmp(dirp1->d_name, dirp2->d_name)==0)
            {
                printf("Datoteka %s, %ld iz prvog direktorijuma je ista kao datoteka %s, %ld iz drugog\n", dirp1->d_name, statbuf1.st_size, dirp2->d_name, statbuf2.st_size);
            }

        }
        rewinddir(dp2);
    }
}
