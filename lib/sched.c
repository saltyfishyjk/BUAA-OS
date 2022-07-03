#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion statrting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
void sched_yield(void)
{
	static int times = 0;
	struct Env *e = curenv;
	struct Tcb *t = curtcb;
	static int sched_i = 0;
	if (!t) {
		//printf("in 1\n");
		while (LIST_EMPTY(&tcb_sched_list[sched_i])) {
			sched_i ^= 1;
		//	printf("empty1!\n");
		}
		t = LIST_FIRST(&tcb_sched_list[sched_i]);
		times = t->tcb_pri;
		times -= 1;
		//printf("now tcb is 0x%x\n",t->thread_id);
		//printf("now t.pc is 0x%x\n",t->tcb_tf.pc);
		env_run(t);
	} else if (t->tcb_status != ENV_RUNNABLE) {
		//printf("in 2, tcbid is 0x%x\n",t->thread_id);
		while (LIST_EMPTY(&tcb_sched_list[sched_i])) {
			sched_i ^= 1;
		//	printf("empty2!\n");
		}
		t = LIST_FIRST(&tcb_sched_list[sched_i]);
		times = t->tcb_pri;
		times -= 1;
		//printf("now threadid is 0x%x\n",t->thread_id);
		env_run(t);
	}
	if (times <= 0) {
		//printf("in 3, tcbid is 0x%x\n",t->thread_id);
		LIST_REMOVE(t,tcb_sched_link);
		LIST_INSERT_HEAD(&tcb_sched_list[sched_i^1],t,tcb_sched_link);
		while (LIST_EMPTY(&tcb_sched_list[sched_i])) {
			sched_i ^= 1;
			//printf("empty3!\n");
		}
		t = LIST_FIRST(&tcb_sched_list[sched_i]);
		times = t->tcb_pri;
		times -= 1;
		//printf("now thread id is 0x%x\n",t->thread_id);
		env_run(t);
	} else {
		//printf("in 4, tcbid is 0x%x\n",t->thread_id);
		times -= 1;
		env_run(t);
	}
}
