/*
Name:               Mohammad Ehtesham
OneCard:            1272185
ID:                 mehtesha
Lecture Section:    EB1
Instructor's Name:  Jacqueline Smith
Lab Section:        EH01
TA's name:          Rong Feng           
*/
#include "functions.h"

void open_pipe(const char* Exec_name){
    executable = popen(Exec_name, "w");
    if (executable == NULL){
        fprintf(stderr, "Could not open pipe %s\n", Exec_name);
    }
}

void close_pipe(){
    pclose(executable);
}

void set_red(){
    fprintf(executable, "setColor 255 0 0\n");
}

void draw_line(points A,points B){
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n",lround(A.x),lround(A.y),lround(B.x),lround(B.y));
}

void erase_line(points A,points B){
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",lround(A.x),lround(A.y),lround(B.x),lround(B.y));
}

double angle_to_radian(long angle){
    double radian;
    radian = angle*PI/180.0;
    return radian;
}

void rotate_point_by_ten_degree_CW(points* P){
    double rotated_x,rotated_y;
    rotated_x = (P->x*cos(angle_to_radian(10)))-(P->y*sin(angle_to_radian(10)));
    rotated_y = (P->x*sin(angle_to_radian(10)))+(P->y*cos(angle_to_radian(10)));
    P->x=rotated_x;
    P->y=rotated_y;
}

void rotate_point_by_ten_degree_CCW(points* P){
    double rotated_x,rotated_y;
    rotated_x = (P->x*cos(angle_to_radian(350)))-(P->y*sin(angle_to_radian(350)));
    rotated_y = (P->x*sin(angle_to_radian(350)))+(P->y*cos(angle_to_radian(350)));
    P->x=rotated_x;
    P->y=rotated_y;
}

void draw_ship(){
    draw_line(ship[0],ship[1]);
    draw_line(ship[0],ship[2]);
    draw_line(ship[1],ship[3]);
    draw_line(ship[2],ship[3]);
}
void erase_ship(){
    erase_line(ship[0],ship[1]);
    erase_line(ship[0],ship[2]);
    erase_line(ship[1],ship[3]);
    erase_line(ship[2],ship[3]);
}

void draw_thrust(){
    draw_line(ship[2],ship[4]);
    draw_line(ship[3],ship[4]);
}
void erase_thrust(){
    erase_line(ship[2],ship[4]);
    erase_line(ship[3],ship[4]);
}

void draw_landscape(FILE* ls){
    memset(LANDSCAPE,0,25*sizeof(points));
    long index=0;
    long x_value,y_value;
    char line[257];
    memset(line,0,257);
    while (fgets(line,257,ls) != NULL){
        sscanf(line,"%ld%ld",&x_value,&y_value);
        memset(line,0,257);
        LANDSCAPE[index].x=x_value;
        LANDSCAPE[index].y=y_value;
        index++;
    }
    long prev_index=0;
    point_cnt=index;
    for(prev_index=0;prev_index<index-1;prev_index++){
        draw_line(LANDSCAPE[prev_index],LANDSCAPE[prev_index+1]);
    }
}

void setup_curses(){
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
}

void unset_curses(){
  keypad(stdscr, false);
  nodelay(stdscr, false);
  nocbreak();
  echo();
  endwin();
}

void handle_timeout(int signal){
  static int called = 0;

  called++;

  if (signal == SIGALRM){
    struct itimerval timer;
    if (getitimer(ITIMER_REAL, &timer) == -1){
        exit(EXIT_FAILURE);
    }
    sigset_t old_mask;
    if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
        exit(EXIT_FAILURE);
    }
    int ship_points=0;
    erase_ship();
    erase_thrust();
    for(ship_points=0;ship_points<5;ship_points++){
        if(thrust_switch==0){
            ship[ship_points].y+=(yV*0.05)+(0.5*gravity*0.05*0.05);
        }
        else if(thrust_switch==1){
            ship[ship_points].y+=(yV*0.05)+(0.5*(gravity+(thrust*sin(angle_to_radian(ship_degree))))*0.05*0.05);
        }
        if(xV!=0){
            ship[ship_points].x+=(xV*0.05)+(0.5*(thrust*cos(angle_to_radian(ship_degree)))*0.05*0.05);
        }
    }
    draw_ship();
    if(thrust_switch==0){
        yV+=gravity*0.05;
    }
    else if(thrust_switch==1){
        draw_thrust();
        yV+=0.05*(gravity+(thrust*sin(angle_to_radian(ship_degree))));
        xV+=0.05*thrust*cos(angle_to_radian(ship_degree));
        thrust_switch=0;
        if(I==1){
            fuel-=0.05;
            erase_fuel_rec(fuel);
        }
    }

    fflush(executable);
    erase_thrust();
    int l_index=0;
    int land_index;
    for(l_index=0;l_index<point_cnt-1;l_index++){
        if((check_intersection(LANDSCAPE[l_index],LANDSCAPE[l_index+1],ship[0],ship[1])==1)||(check_intersection(LANDSCAPE[l_index],LANDSCAPE[l_index+1],ship[0],ship[2])==1)||(check_intersection(LANDSCAPE[l_index],LANDSCAPE[l_index+1],ship[1],ship[3])==1)||(check_intersection(LANDSCAPE[l_index],LANDSCAPE[l_index+1],ship[2],ship[3])==1)){
            over=1;
            land_index=l_index;
            break;
        }
    }
    // (I = 1)
    if(I==1){
        move(7,10);
        double TIME_USED=called/20;
        printw("Time used:  %lf",TIME_USED);
        move(8,10);
        if(fuel>0){
            printw("Fuel amount:  %lf",fuel);
        }
	move(9,10);
	printw("Keep landing velocity less than 30 for successful landing");
	move(10,10);
	printw("Landing Velocity: %lf", yV);
	move(11,10);
	printw("Points:\n30 - Between 0 & 5 (Not for the faint of heart!!!)\n20 - Between 5 & 25\n30 - Between 25 & 30 (Not for the faint of heart!!!)");
        refresh();
    }
    // Displaying results after completion
    if (over==1){
        timer.it_interval.tv_usec = 0;
        timer.it_value.tv_usec = 0;
	fprintf(executable,"end\n");
        if(I==0){move(7,10);}
        if(I==1){move(15,10);}
        if((yV<30)&&(ship_degree==90)&&(LANDSCAPE[land_index].y==LANDSCAPE[land_index+1].y)){
	    printw("Success!!! Final Land Speed: %lf\n", yV);
		if(((yV>25.00) & (yV<30.00)) || ((yV>0.00) & (yV<5.00))){
		printw("Final score: 30. You're the BAWS!");
		}
		else if((yV>5.00) & (yV<25.00)){
		printw("Final score: 20. Not bad, human!");
		}
        }
        else{
            printw("Crashed!!! Final Land Speed: %lf. Land Speed needs to be less than 30 for success. Better luck next time!", yV);
        }
    }

    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
        exit(EXIT_FAILURE);
    }

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1){
        exit(EXIT_FAILURE);
    }
  }

}

void blocking_signal_for_rotate(int direction){
  sigset_t old_mask;
  if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
      exit(EXIT_FAILURE);
  }
  erase_ship();
  int idx=1;
  double min_x=ship[0].x;
  double max_x=ship[0].x;
  double min_y=ship[0].y;
  double max_y=ship[0].y;
  for(idx=1;idx<4;idx++){
      if(ship[idx].x<min_x){
          min_x=ship[idx].x;
      }
      if(ship[idx].x>max_x){
          max_x=ship[idx].x;
      }
      if(ship[idx].y<min_y){
          min_y=ship[idx].y;
      }
      if(ship[idx].y>max_y){
          max_y=ship[idx].y;
      }
  }
  double origin_center_x=(min_x + max_x) / 2.0;
  double origin_center_y=(min_y + max_y) / 2.0;
  points R_ship[5];
  idx=0;
  for(idx=0;idx<5;idx++){
      R_ship[idx].x=ship[idx].x-origin_center_x;
      R_ship[idx].y=ship[idx].y-origin_center_y;
      if(direction==0){
          rotate_point_by_ten_degree_CW(&R_ship[idx]);
      }
      else if(direction==1){
          rotate_point_by_ten_degree_CCW(&R_ship[idx]);
      }
      ship[idx].x=R_ship[idx].x+origin_center_x;
      ship[idx].y=R_ship[idx].y+origin_center_y;
  }
  if(direction==0){
      if(ship_degree==360){
          ship_degree=0;
      } 
      ship_degree+=10;
  }
  if(direction==1){
      if(ship_degree==0){
          ship_degree=360;
      } 
      ship_degree-=10;
  }
  draw_ship();
  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
      exit(EXIT_FAILURE);
  }
}

void draw_fuel_rec(){
    set_red();
    points level[2];
    level[0].x=580;
    level[1].x=590;
    level[0].y=30;
    level[1].y=30;
    long fuel_index=0;
    for(fuel_index=0;fuel_index<50;fuel_index++){
        draw_line(level[0],level[1]);
        level[0].y++;
        level[1].y++;
    }
    points sides[4];
    sides[0].x=579;
    sides[0].y=29;
    sides[1].x=591;
    sides[1].y=29;
    sides[2].x=579;
    sides[2].y=80;
    sides[3].x=591;
    sides[3].y=80;
    draw_line(sides[0],sides[1]);
    draw_line(sides[0],sides[2]);
    draw_line(sides[1],sides[3]);
    draw_line(sides[2],sides[3]);
	fprintf(executable, "setColor 0 0 0\n");
}

void erase_fuel_rec(double fuel){
    points rmlevel[2];
    rmlevel[0].x=580;
    rmlevel[1].x=590;
    rmlevel[0].y=80-(5*fuel);
    rmlevel[1].y=80-(5*fuel);
    erase_line(rmlevel[0],rmlevel[1]);
}

int check_intersection(points A,points B,points C,points D){
    double dist_AB, thecos, thesin, newX, ABpos;
    if ((A.x==B.x && A.y==B.y)||(C.x==D.x && C.y==D.y)){
        return 0;
    }
    if ((A.x==C.x && A.y==C.y)||(B.x==C.x && B.y==C.y)||(A.x==D.x && A.y==D.y)||(B.x==D.x && B.y==D.y)){
        return 0;
    }

    points cpy_A=A;
    points cpy_B=B;
    points cpy_C=C;
    points cpy_D=D;

    cpy_B.x-=cpy_A.x;
    cpy_B.y-=cpy_A.y;
    cpy_C.x-=cpy_A.x;
    cpy_C.y-=cpy_A.y;
    cpy_D.x-=cpy_A.x;
    cpy_D.y-=cpy_A.y;

    dist_AB=sqrt((cpy_B.x*cpy_B.x)+(cpy_B.y*cpy_B.y));
    thecos=cpy_B.x/dist_AB;
    thesin=cpy_B.y/dist_AB;
    newX=(cpy_C.x*thecos)+(cpy_C.y*thesin);

    cpy_C.y=(cpy_C.y*thecos)-(cpy_C.x*thesin);
    cpy_C.x=newX;
    newX=(cpy_D.x*thecos)+(cpy_D.y*thesin);
    cpy_D.y=(cpy_D.y*thecos)-(cpy_D.x*thesin);
    cpy_D.x=newX;

    if ((cpy_C.y<0. && cpy_D.y<0.)||(cpy_C.y>=0. && cpy_D.y>=0.)){
        return 0;
    }
    ABpos=cpy_D.x+(cpy_C.x-cpy_D.x)*cpy_D.y/(cpy_D.y-cpy_C.y);

    if (ABpos<0.||ABpos>dist_AB){
        return 0;
    }
    return 1;
}

