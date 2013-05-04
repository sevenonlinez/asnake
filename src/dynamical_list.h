#ifndef _DYNAMICAL_LIST_H
#define _DYNAMICAL_LIST_H

struct snake {
    int xachse;
    int yachse;
    struct snake *next;
    struct snake *previous;
};

extern struct snake *anfang;
extern struct snake *next;
extern struct snake *ende;


void anhaengen(int xachse, int yachse);
void del_first(char **playfield);

#endif // _DYNAMICAL_LIST_H
