/*
Name:               Mohammad Ehtesham
OneCard:	    1272185
ID:                 mehtesha
Lecture Section:    EB1
Instructor's Name:  Jacqueline Smith
Lab Section:        EH01
TA's name:          Rong Feng           
*/

#ifndef LANDER_H
#define LANDER_H

typedef struct point{
    double x;
    double y;
}points;

void open_pipe(const char* Exec_name);

void close_pipe();

void draw_line(points A,points B);

void erase_line(points A,points B);

double angle_to_radian(long angle);

void rotate_point_by_ten_degree_CW(points* P);

void rotate_point_by_ten_degree_CCW(points* P);

void draw_ship();

void draw_thrust();

void erase_ship();

void erase_thrust();

void draw_landscape(FILE* ls);

//stdio function prototypes

FILE *popen(const char *command, const char *type);

int pclose(FILE *stream);

//curses function prototypes

void setup_curses();

void unset_curses();

//timer function prototypes

void handle_timeout(int signal);

void blocking_signal_for_rotate(int direction);

//intersection function prototypes

int check_intersection(points A,points B,points C,points D);

//improvement function prototypes

void draw_fuel_rec();

void erase_fuel_rec(double fuel);

#endif
