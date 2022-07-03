#include "lib.h"
void *test(void *arg) {
	int arg1 = ((int *)arg)[0];
	int arg2 = ((int *)arg)[1];
	int arg3 = ((int *)arg)[2];
	int c;
	int *father_a = (int *)((int *)arg)[3];
	int *father_b = (int *)((int *)arg)[4];
	writef("arg 1 is %d\n",arg1);
	writef("arg 2 is %d\n",arg2);
	writef("arg 3 is %d\n",arg3);
	writef("a is %d\n",*father_a);
	++(*father_b);
	writef("b is change\n");
	while (1) {
		writef("");
		c = *father_a;
		if (*father_a != 1)
			break;
	}
	writef("a is %d\n",c);
}
void umain() {
	int a;
	int b;
	a = 0;
	b = 0;
	int c;
	++a;
	int thread;
	int args[5];
	args[0] = 1;
	args[1] = 2;
	args[2] = 3;
	args[3] = &a;
	args[4] = &b;
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
	while(1);
}


