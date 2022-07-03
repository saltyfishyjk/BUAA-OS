#include "lib.h"

void *test1(void *args) {
	u_int father_threadid = ((u_int *)args)[0];
	u_int *ret;
	writef("son1 is working\n");
	if (pthread_join(father_threadid,&ret) < 0) {
		writef("son1 fail join!\n");
		pthread_exit(0);
	}
	writef("son1: father is end, ret is %d\n",*ret);
}

void *test2(void *args) {
	u_int father_threadid = ((u_int *)args)[0];
	u_int *ret;
	writef("son2 is working\n");
	if (pthread_join(father_threadid,&ret) < 0) {
		writef("son2 fail join!\n");
		pthread_exit(0);
	}

	writef("son2: father is end, ret is %d\n",*ret);
}

void *test3(void *args) {
	u_int father_threadid = ((u_int *)args)[0];
	u_int *ret;
	writef("son3 is working\n");
	if (pthread_join(father_threadid,&ret) < 0) {
		writef("son3 fail join!\n");
		pthread_exit(0);
	}
	writef("son3: father is end, ret is %d\n",*ret);
}

void *test4(void *args) {
	pthread_cancel(((u_int *)args)[0]);
}

void umain() {
	u_int a[1];
	u_int ret = -2;
	a[0] = syscall_getthreadid();
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_detach(a[0]);
	pthread_create(&thread1,NULL,test1,(void *)a);
	pthread_create(&thread2,NULL,test2,(void *)a);
	pthread_create(&thread3,NULL,test3,(void *)a);
	int b = 0;
	while (b < 10) {
		++b;
		writef("now b is %d\n",b);
		syscall_yield();
	}
	//pthread_exit(&ret);
	//pthread_setcancelstate(THREAD_CAN_BE_CANCELED,0);
	//pthread_create(&thread4,NULL,test4,(void *)a);
	//while (1);
}
