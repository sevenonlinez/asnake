#ifndef _DYNAMICAL_LIST_H
#define _DYNAMICAL_LIST_H

struct snake_element {
    int vector[2];
    struct snake_element *next;
    struct snake_element *previous;
};

struct snake {

};

extern struct snake_element *anfang;
extern struct snake_element *next;
extern struct snake_element *ende;


void anhaengen(int vector[]);
void del_first(char **playfield);

#endif // _DYNAMICAL_LIST_H
