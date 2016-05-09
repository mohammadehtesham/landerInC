lander: lander.o functions.o
	gcc -Wall -std=c99 -o lander lander.o functions.o -lm -l curses

lander.o: lander.c lander.h
	gcc -Wall -std=c99 -c lander.c

functions.o: functions.c functions.h lander.h
	gcc -Wall -std=c99 -c functions.c

clean:
	-rm -f *.o lander core

tar:
	tar -cvf submit.tar lander.c lander.h functions.c functions.h Makefile README landscape.txt
