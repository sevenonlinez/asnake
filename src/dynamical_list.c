#include <stdio.h>
#include <stdlib.h>
#include "dynamical_list.h"

struct snake *anfang=NULL;
struct snake *next=NULL;
struct snake *ende=NULL;


void anhaengen(int vector[]) {
    int anz_vector = sizeof(vector) / sizeof(int);
    struct snake *zeiger, *zeiger1;

    /* Wurde schon Speicher für den ende-Zeiger bereitgestellt */
    if(ende == NULL) {
        if((ende = malloc(sizeof(struct snake))) == NULL) {
            printf("Konnte keinen Speicherplatz für ende reservieren\n");
            exit(0);
        }
    }



    if(anfang == NULL) {
        /* Reservieren von Speicherplatz für Struktur
     * für das erste Element der Liste*/
        if((anfang = malloc(sizeof(struct snake))) == NULL) {
            fprintf(stderr, "Kein Speicherplatz vorhanden für anfang\n");
            return;
        }
        int i;
        for(i=0; i<anz_vector; i++) {
            anfang->vector[i]=vector[i];
        }
        anfang->next=NULL;
        ende=anfang;
        ende->previous=NULL;
    }
    else {
        zeiger=anfang; /* Zeiger auf 1. Element */
        while(zeiger->next != NULL)
            zeiger=zeiger->next;


        /* Speicherplatz für das letzte
     * Element der Liste reservieren und anhängen.   */
        if((zeiger->next =malloc(sizeof(struct snake))) == NULL) {
            fprintf(stderr,"Kein Speicherplatz für das "
                    "letzte Element\n");
            return;
        }

        zeiger1=zeiger;

        zeiger=zeiger->next; /* zeiger auf neuen Speicherplatz */

        int i;
        for(i=0; i<anz_vector; i++) {
            anfang->vector[i]=vector[i];
        }

        zeiger->next=NULL;
        ende=zeiger;
        zeiger->previous=zeiger1;
        zeiger1->next=zeiger;
    }
}

void del_first(char **playfield) {  //Um das 1. Element der Liste zu löschen
    struct snake *zeiger;
    zeiger=anfang->next;
    playfield[anfang->vector[0]][anfang->vector[1]]=' ';
    if(zeiger == NULL) {
        free(anfang);
        anfang=NULL;
        ende=NULL;
        return;
    }
    zeiger->previous=NULL;
    free(anfang);
    anfang=zeiger;

}


