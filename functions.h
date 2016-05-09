#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define _POSIX_C_SOURCE 200112L

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<math.h>
#include<assert.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>
#include"lander.h"

#define PI acos(-1.0)

extern FILE* executable;

extern FILE* ls;

extern points ship[5];

extern points LANDSCAPE[25];

extern int point_cnt;

extern long ship_degree;

extern int thrust_switch,over,I;

extern double fuel;

extern double xV,yV;

extern double gravity,thrust;

extern sigset_t block_mask_g;

void open_pipe(const char* Exec_name);

void close_pipe();

void draw_line(points A, points B);

void erase_line(points A, points B);

double angle_to_radian(long angle);

void rotate_point_by_ten_degree_CW(points* P);

void rotate_point_by_ten_degree_CCW(points* P);

void draw_ship();

void draw_thrust();

void erase_ship();

void erase_thrust();

void draw_landscape(FILE* ls);

void setup_curses();

void unset_curses();

void handle_timeout(int signal);

void blocking_signal_for_rotate(int direction);

int check_intersection(points A,points B,points C,points D);

void draw_fuel_rec();

void erase_fuel_rec(double fuel);

#endif

