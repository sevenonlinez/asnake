#ifndef _MOVE_H
#define _MOVE_H

extern int special_point_active;
extern int hindernis_check;
extern int sync_del_special_point;

void x_move(int richtung, char **playfield);
void y_move(int richtung, char **playfield);
void move_snake(char **playfield);

#endif // _MOVE_H

