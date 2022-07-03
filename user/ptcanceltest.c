#include "lib.h"

void *test(void *args) {
	int oldvalue;
	pthread_setcancelstate(1,&oldvalue);
	writef("oldstate is %d\n",oldvalue);
	while(1);
}

void *test2(void *args) {
	int oldvalue;
	pthread_setcanceltype(1,&oldvalue);
	writef("oldtype is %d\n",oldvalue);
	pthread_setcancelstate(1,&oldvalue);
	writef("oldstate is %d\n",oldvalue);
	while (1) {
		pthread_testcancel();
		writef("son cannot be canceled\n");
		syscall_yield();
	}
}

void umain() {
	int a[3];
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	pthread_t thread;
	if (pthread_create(&thread,NULL,test,(void *)a)) {
		user_panic("create thread fail!\n");
	}
	syscall_yield();
	pthread_cancel(thread);
	if (pthread_create(&thread,NULL,test2,(void *)a)) {
		user_panic("create thread fail 2\n");
	}
	while (pthread_cancel(thread) < 0);
	writef("father now canceled son!\n");
	syscall_yield();
}
