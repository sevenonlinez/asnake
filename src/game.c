#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // für STDIN_FILENO
#include <pthread.h>

#include "parameter.h"
#include "game.h"
#include "move.h"
#include "display.h"
#include "dynamical_list.h"

struct game_t game;

struct parameter {
    char **array;
};

struct koordinaten specialpoint;

extern struct snake_element *anfang;
extern struct snake_element *next;
extern struct snake_element *ende;
int incr[2];

int special_point_active;
char eingabe;

void *read_stdin () {
    while(0 == 0) {
        char tmp;
        if (read (STDIN_FILENO, &tmp, 1) < 1) {
            printf ("Fehler bei read\n");
            restore_tty (STDIN_FILENO);
            exit (EXIT_FAILURE);
        }
        switch (tmp) {
        case 100:	/* Button d */
            if (incr[0] == -1 && incr[1]==0) {
                break;
            }
            else {
                incr[0] = 1;
                incr[1] = 0;
            }
            break;

        case 97:	/* Button a */
            if (incr[0] == 1 && incr[1] == 0) {
                break;
            }

            else {
                incr[0] = -1;
                incr[1] = 0;
            }
            break;
        case 119:   /* Button w */
            if (incr[0] == 0 && incr[1] == 1) {
                break;
            }

            else {
                incr[0] = 0;
                incr[1] = -1;
            }
            break;
        case 115:  /*Button s */
            if (incr[0] == 0 && incr[1] == -1) {
                break;
            }
            else {
                incr[0] = 0;
                incr[1] = 1;
            }
            break;

        case 113: /* Button q */
            eingabe = 'q';
            break;

        default:
            break;

        }

        usleep(game.geschwindigkeit);
    }   /* while loop */
}


void snake_create_playground (char **playfield) {
    int counter;
    int n,m;
    for(n=0; n < len_x-1; n++) {
        for( m=0; m < len_y-1; m++) {
            playfield[n][m]=' ';
        }
    }

    for (counter=0;counter<len_x;counter++) {
        playfield[counter][0]='#';
        playfield[counter][len_y-1]='#';
    }

    for (counter=0;counter<len_y;counter++) {
        playfield[0][counter]='#';
        playfield[len_x-1][counter]='#';
    }

}

void snake_print_out (char **playfield) {
    int j,i;
    for (i=0;i<len_y;i++) {
        for( j=0;j<len_x;j++) {
            printf("%c ", playfield[j][i]);
        }
        printf("\n");
    }
}

void create_snake (char **playfield) {
    int mitte[2];
    mitte[0]= len_x / 2;
    mitte[1] = len_y / 2 -2;

    anhaengen(mitte);

    mitte[0]++;
    anhaengen(mitte);

    mitte[0]++;
    anhaengen(mitte);

    mitte[0]++;
    anhaengen(mitte);


    struct snake_element *zeiger;

    zeiger=anfang;

    while( zeiger != NULL) {
        int x=zeiger->vector[0];
        int y=zeiger->vector[1];
        playfield[x][y]='@';
        zeiger=zeiger->next;
    }
}

int zufallsauswahl (int minimum, int maximum) {
    int bereich = maximum - minimum + 1;
    int zahl = minimum + ( rand() % bereich );
    return zahl;
}

void special_point (/*pthread_t th1, pthread_t th2,*/ char **playfield) {
    pthread_t th1,th2;
    struct parameter *f;
    f = (struct parameter *)malloc(sizeof(struct parameter));
    if( f==NULL) {
        printf("Konnte keinen Speicher reservieren...!!!\n");
    }
    f->array=playfield;

    do  {
        specialpoint.x= zufallsauswahl(1,len_x);

        specialpoint.y= zufallsauswahl(1,len_y);

    } while ( playfield[specialpoint.x][specialpoint.y]=='@' \
              || playfield[specialpoint.x][specialpoint.y]== '*'	\
              || playfield[specialpoint.x][specialpoint.y]== '#'	\
              || playfield[specialpoint.x][specialpoint.y]== '$');

    playfield[specialpoint.x][specialpoint.y]='$';
    special_point_active=1;
    sync_del_special_point=0;
    if( pthread_create(&th2,NULL, &special_point_blink,f) != 0) {
        printf ("Konnte keinen Thread erzeugen\n");
        exit (EXIT_FAILURE);
    }

    if (pthread_create (&th1,NULL,&del_special_point,f) != 0) {
        printf ("Konnte keinen Thread erzeugen\n");
        exit (EXIT_FAILURE);
    }
}

void *special_point_blink(void *param) {
    struct parameter *f = (struct parameter *)param;
    //   int sync_blinken=0;

    while(special_point_active==1 && sync_del_special_point==0 ) {
        if(f->array[specialpoint.x][specialpoint.y]=='$') {
            f->array[specialpoint.x][specialpoint.y]=' ';
        }
        else if(f->array[specialpoint.x][specialpoint.y]) {
            f->array[specialpoint.x][specialpoint.y]='$';
        }
        usleep(2*game.geschwindigkeit);
    }
    pthread_exit(NULL);
}

void *del_special_point(void *param) {
    struct parameter *f = (struct parameter *)param;
    sync_del_special_point=0;

    struct snake_element *zeiger;
    zeiger=anfang; /* Zeiger auf 1. Element */
    while(zeiger->next != NULL)
        zeiger=zeiger->next;

    int len=0;

    if(zeiger->vector[0]>specialpoint.x) {
        len+=(zeiger->vector[0]-specialpoint.x);
    }
    else if(zeiger->vector[0]<=specialpoint.x) {
        len+=(specialpoint.x-zeiger->vector[0]);
    }

    if(zeiger->vector[1]>specialpoint.y) {
        len+=(zeiger->vector[1]-specialpoint.y);
    }
    else if(zeiger->vector[1]<=specialpoint.y) {
        len+=(specialpoint.y-zeiger->vector[1]);
    }
    len+=10;
    len*=game.geschwindigkeit;

    usleep(len);

    if (sync_del_special_point==0) {
        f->array[specialpoint.x][specialpoint.y]=' ';
        sync_del_special_point=1;
    }
    pthread_exit(NULL);
}


void game_over() {
    system("clear");
    printf("SCORE:\t\t%i\n\n", game.score);
    printf(" _______  _______  __   __  _______ \n");
    printf("|       ||   _   ||  |_|  ||       |\n");
    printf("|    ___||  |_|  ||       ||    ___|\n");
    printf("|   | __ |       ||       ||   |___ \n");
    printf("|   ||  ||       ||       ||    ___|\n");
    printf("|   |_| ||   _   || ||_|| ||   |___ \n");
    printf("|_______||__| |__||_|   |_||_______|\n");
    printf(" _______  __   __  _______  _______ \n");
    printf("|       ||  | |  ||       ||    _ | \n");
    printf("|   _   ||  |_|  ||    ___||   | || \n");
    printf("|  | |  ||       ||   |___ |   |_|| \n");
    printf("|  |_|  ||       ||    ___||    __ |\n");
    printf("|       | |     | |   |___ |   |  ||\n");
    printf("|_______|  |___|  |_______||___|  ||\n\n");
    printf("\n\n");
    restore_tty (STDIN_FILENO);
    exit(0);
}

void create_hindernis(char **playfield) {
    int x,y;
    do {
        if(game.level>3) {
            x=zufallsauswahl(2,len_x-3);
            y=zufallsauswahl(2,len_y-3);
        }
        else {
            x=zufallsauswahl(1,len_x-2);
            y=zufallsauswahl(1,len_y-2);
        }
    } while(playfield[x][y]=='@' || playfield[x][y]=='$' || x%2!=0 );

    playfield[x][y]='*';
}


