/*
Name:               Mohammad Ehtesham
OneCard:	    1272185
ID:	            mehtesha
Lecture Section:    EB1
Instructor's Name:  Jacqueline Smith
Lab Section:        EH01
TA's name:          Rong Feng           
*/
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <assert.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>

#include "lander.h"

#define PI acos(-1.0)

//Defining pipe stream
FILE* executable;
//Defining landscape file stream
FILE* ls;
const char Exec_name[] = "java -jar Sketchpad.jar";

//Ship points (to draw and move). Points 1-4 define points of ship. Point 5 defines point for drawing flame
points ship[5];

//Angle of ship
long ship_degree;

//Landscape points. Assignment puts limit of 20 points.
//Adding more for proper execution in case of more points.
points LANDSCAPE[25];

int point_cnt;

//Signal Blocker - Please view aknowledgements for code for timer and blocking signals
sigset_t block_mask_g;

//Initializing Switches for thrust, game over and improvement flag
int thrust_switch,over,I;
// Fuel (used for improvement with flag -i), x and y velocities, gravity and thrust as issued by the command line
double fuel, xV, yV, gravity, thrust;

int main(int argc,char *argv[]){
    I=0;
    if (argc!=7){
	if (argc!=8){
	    fprintf(stderr, "Incorrect number of arguments entered\n");
	    fprintf(stderr, "Enter -f landscape.txt-g GRAVITY -t THRUST\n");
	return 0;
	}
    }

    int argv_i=1;
    for(argv_i=1; argv_i < argc; argv_i++) {
	if(argv[argv_i][0]=='-'){
		if(strlen(argv[argv_i])==2){
			if((argv[argv_i][1])=='g'){
				gravity=atof(argv[argv_i+1]);
				if (gravity > 20 || gravity < 0){
					fprintf(stderr, "Gravity is out of bounds. Set between 0 and 20\n");
					exit(EXIT_FAILURE);
					}
			}
			else if((argv[argv_i][1])=='t'){
				thrust=atof(argv[argv_i+1]);
				if (thrust < -20 || thrust > 0){
                                        fprintf(stderr, "Thrust is out of bounds. Set between -20 and 0\n");
                                        exit(EXIT_FAILURE);
                                        }
			}
			else if((argv[argv_i][1])=='f'){
				ls=fopen(argv[argv_i+1],"r");
				if (ls == NULL){
					fprintf(stderr, "Please provide landscape file\n");
					exit(EXIT_FAILURE);
				}
			}
                        else if((argv[argv_i][1])=='i'){
                                I=1;
                        }
		}
	}
    }

    fuel=10;
    ship_degree=90;
    // curses set up------------------------------------------
    setup_curses();
    open_pipe(Exec_name);
    move(5, 10);
    printw("Press any key to start.\n");
    printw("          (Then press arrow keys to rotate, space for thrust, 'q' to quit)");
    refresh();
    int cmd = getch();
	
    nodelay(stdscr, true);
    erase();
    move(5, 0);
    printw("Left arrow key rotate counter-clockwise, right clockwise, space for thurst & q to quit.");
	
    //Initializing the space in 2D
    ship[0].x=315;
    ship[0].y=20;
    ship[1].x=325;
    ship[1].y=20;
    ship[2].x=305;
    ship[2].y=50;
    ship[3].x=335;
    ship[3].y=50;
    ship[4].x=320;
    ship[4].y=60;

    over=0;
    thrust_switch=0;
    xV=0;yV=0;
    draw_landscape(ls);
    if(I==1){
        draw_fuel_rec();
    }

    sigemptyset(&block_mask_g);
    sigaddset(&block_mask_g, SIGALRM);
    struct sigaction handler;
    handler.sa_handler = handle_timeout;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    if (sigaction(SIGALRM, &handler, NULL) == -1){
        exit(EXIT_FAILURE);
    }
    struct itimerval timer;
        struct timeval time_delay;
        time_delay.tv_sec = 0;
        time_delay.tv_usec = 50000;
        timer.it_interval = time_delay;
        struct timeval start;
        start.tv_sec = 0;
        start.tv_usec = 50000;
        timer.it_value = start;
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1){
        exit(EXIT_FAILURE);
    }
    while(1){
        cmd = getch();
        if (cmd != ERR){
            if (cmd == KEY_LEFT){
                if(over==0){
                    blocking_signal_for_rotate(1);
                }
            }
            else if(cmd == KEY_RIGHT){
                if(over==0){
                    blocking_signal_for_rotate(0);
                }
            }
            else if(cmd == ' '){
                if(I==0){
                    thrust_switch=1;
                }
                else if(I==1){
                    if(fuel<=0){
                        move(8,10);
                        refresh();
                        printw("No more fuel!!!");
                    }
                    else if(fuel>0){
                        thrust_switch=1;
                    }
                }
            }
            else if(cmd == 'q'){
                sigset_t old_mask;
                if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
                    exit(EXIT_FAILURE);
                }
                break;
            }
        }
    }
    if(executable!=NULL&&over==0){
        fprintf(executable,"end\n");
    }
    unset_curses();
    fclose(ls);
    close_pipe();
    exit(EXIT_SUCCESS);
}
