/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji se ne moze prekinuti jednim pritiskom na kombinaciju tastera Ctrl-C,
već sa 3 uzastopna pritiska ove kombinacije tastera u roku od 3 sekunde. Ukoliko od prvog pritiska kmbinacije tastera Ctrl-C protekne više
od 3 sekunde a korisnik nije 3 puta pritisnuo ovu kombinaciju tastera program na ekranu štampa poruku da se prekid programa resetuje i cela procedura
detekcije 3 pritiska u roku od 3 sekunde počinje ispočetka.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int ctrl_count=0;

void alarmFja(int sig_num)
{
    ctrl_count=0;
    printf("\nProslo je 3 sekunde, proces se restuje...\n");
//     alarm(0);
}

void funkcija(int sig_num)
{
    signal(SIGINT, funkcija);

    alarm(3);

    ctrl_count++;

    if(ctrl_count>=2)
    {
        ctrl_count=0;
        signal(SIGINT, SIG_DFL);
    }
}

int main()
{
    signal(SIGINT, funkcija);
    signal(SIGALRM, alarmFja);

    while(1)
        pause();

    return 0;
}
