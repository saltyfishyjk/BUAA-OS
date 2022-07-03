#include "lib.h"
int a;
int b;
void *test(void *arg) {
	int arg1 = ((int *)arg)[0];
	int arg2 = ((int *)arg)[1];
	int arg3 = ((int *)arg)[2];
	int c;
	writef("arg 1 is %d\n",arg1);
	writef("arg 2 is %d\n",arg2);
	writef("arg 3 is %d\n",arg3);
	writef("a is %d\n",a);
	++b;
	writef("b is change\n");
	while (1) {
		writef("");
		if (a != 1)
			break;
	}
	writef("a is %d\n",a);
}
void umain() {
	a = 0;
	b = 0;
	int c;
	++a;
	int thread;
	int args[3];
	args[0] = 1;
	args[1] = 2;
	args[2] = 3;
	pthread_t son;
	thread = pthread_create(&son,NULL,test,(void *) args);
	writef("create successful\n");
	if (!thread) {
		while (1) {
			writef("");
			if (b != 0)
				break;
		}
		++a;
		writef("I am out\n");	
	}	
}


