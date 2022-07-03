#include "lib.h"
void *buy(void *args) {
	sem_t *mutex = (sem_t *)((u_int *)args)[0];
	int *b = (int *)((u_int *)args)[1];
	int son = (int)((u_int *)args)[2];
	int c;
	int exitflag = 0;
	while (1) {
		sem_wait(mutex);
		//writef("son%d got mutex\n",son);
		if (*b > 0) {
			c = *b;
			*b = *b - 1;
			writef("son%d buy ticket %d, now have %d tickets\n",son,c,*b);
		}
		if (*b == 0) {
			exitflag = 1;
		} else if (*b < 0) {
			user_panic("panic at son%d, tickets are %d\n",son,*b);
		}
		//writef("son%d free mutex\n",son);
		sem_post(mutex);
		if (exitflag) {
			break;
		}
	}
	pthread_exit(0);
}
void umain() {
	u_int arg1[3];
	u_int arg2[3];
	u_int arg3[3];
	sem_t mutex;
	sem_init(&mutex,0,1);
	arg1[0] = &mutex;
	arg2[0] = &mutex;
	arg3[0] = &mutex;
	int sum = 100;
	arg1[1] = &sum;
	arg2[1] = &sum;
	arg3[1] = &sum;
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	arg1[2] = 1;
	pthread_create(&thread1,NULL,buy,(void *)arg1);
	arg2[2] = 2;
	pthread_create(&thread2,NULL,buy,(void *)arg2);
	arg3[2] = 3;
	pthread_create(&thread3,NULL,buy,(void *)arg3);
}
