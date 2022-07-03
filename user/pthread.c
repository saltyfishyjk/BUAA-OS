#include "lib.h"
#include <error.h>
#include <mmu.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void * (*start_rountine)(void *), void *arg) {
	int newthread = syscall_thread_alloc();
	if (newthread < 0) {
		thread = 0;
		return newthread;
	}
	struct Tcb *t = &env->env_threads[newthread];
	t->tcb_tf.regs[29] = USTACKTOP - 4*BY2PG*newthread;
	t->tcb_tf.pc = start_rountine;
	t->tcb_tf.regs[29] -= 4;
	t->tcb_tf.regs[4] = arg;
	t->tcb_tf.regs[31] = exit;
	//struct Tcb **threadtcb = (struct Tcb **)t->tcb_tf.regs[29];
	//*threadtcb = t;
	syscall_set_thread_status(t->thread_id,ENV_RUNNABLE);
	*thread = t->thread_id;
	return 0;
}

void pthread_exit(void *value_ptr) {
	u_int threadid = syscall_getthreadid();
	struct Tcb *t = &env->env_threads[threadid&0x7];
	t->tcb_exit_ptr = value_ptr;
	syscall_thread_destroy(threadid);
}

int pthread_setcancelstate(int state, int *oldvalue) {
	u_int threadid = syscall_getthreadid();
	struct Tcb *t = &env->env_threads[threadid&0x7];
	if ((state != THREAD_CAN_BE_CANCELED) & (state != THREAD_CANNOT_BE_CANCELED)) {
		return -1;
	}
	if (t->thread_id != threadid) {
		return -1;
	}
	if (oldvalue != 0) {
		*oldvalue = t->tcb_cancelstate;
	}
	t->tcb_cancelstate = state;
	return 0;
}

int pthread_setcanceltype(int type, int *oldvalue) {
	u_int threadid = syscall_getthreadid();
	struct Tcb *t = &env->env_threads[threadid&0x7];
	if ((type != THREAD_CANCEL_IMI) & (type != THREAD_CANCEL_POINT)) {
		return -1;
	}
	if (t->thread_id != threadid) {
		return -1;
	}
	if (oldvalue != 0) {
		*oldvalue = t->tcb_canceltype;
	}
	t->tcb_canceltype = type;
	return 0;
}

void pthread_testcancel() {
	u_int threadid = syscall_getthreadid();
	struct Tcb *t = &env->env_threads[threadid&0x7];
	if (t->thread_id != threadid) {
		user_panic("panic at pthread_testcancel!\n");
	}
	if ((t->tcb_canceled)&(t->tcb_cancelstate == THREAD_CAN_BE_CANCELED)&(t->tcb_canceltype == THREAD_CANCEL_POINT)) {
		t->tcb_exit_value = -THREAD_CANCELED_EXIT;
		syscall_thread_destroy(t->thread_id);
	}
}

int pthread_cancel(pthread_t thread) {
	struct Tcb *t = &env->env_threads[thread&0x7];
	if ((t->thread_id != thread)|(t->tcb_status == ENV_FREE)) {
		return -E_THREAD_NOTFOUND;
	}
	if (t->tcb_cancelstate == THREAD_CANNOT_BE_CANCELED) {
		return -E_THREAD_CANNOTCANCEL;
	}
	t->tcb_exit_value = -THREAD_CANCELED_EXIT;
	if (t->tcb_canceltype == THREAD_CANCEL_IMI) {
		syscall_thread_destroy(thread);
	} else {
		t->tcb_canceled = 1;
	}
	return 0;
}

int pthread_detach(pthread_t thread) {
	struct Tcb *t = &env->env_threads[thread&0x7];
	int r;
	int i;
	if (t->thread_id != thread) {
		return -E_THREAD_NOTFOUND;
	}
	if (t->tcb_status == ENV_FREE) {
		u_int sp = USTACKTOP - BY2PG*4*(thread&0x7);
		for(i = 1; i <= 4; ++i) {
			r = syscall_mem_unmap(0,sp-i*BY2PG);
			if (r < 0)
				return r;
		}
		user_bzero(t,sizeof(struct Tcb));
	} else {
		t->tcb_detach = 1;
	}
	return 0;
}

int pthread_join(pthread_t thread, void **value_ptr) {
	int r = syscall_thread_join(thread,value_ptr);
	return r;
}


