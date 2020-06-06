#include "lib.h"


void umain()
{
	int a = 0;
	/*int id = 0;

	if ((id = fork()) == 0) {
		if ((id = fork()) == 0) {
			a += 3;

			for (;;) {
				writef("\t\tthis is child2 :a:%d\n", a);
			}
		}

		a += 2;

		for (;;) {
			writef("\tthis is child :a:%d\n", a);
		}
	}

	a++;

	for (;;) {
		writef("this is father: a:%d\n", a);
	}*/
	syscall_init_PV_var(1);
	while(a < 10) {
		syscall_P(1);
		writef("a %d \n", a);
		a++;
	}
}
