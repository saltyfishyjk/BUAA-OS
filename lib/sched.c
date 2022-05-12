#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.15 ***/
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    static struct Env *e = NULL;
	//printf("\ncurenv->env_id : %d\n", e->env_id);
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
     if (count == 0 || e == NULL || e->env_status != ENV_RUNNABLE) {
         if (e != NULL) {
             LIST_REMOVE(e, env_sched_link);
             if (e->env_status != ENV_FREE) {
				 if (e->env_pri % 2 != 0) {
					 int next = (point + 1) % 3;
					 LIST_INSERT_TAIL(&env_sched_list[next], e, env_sched_link);
				 } else {
					 int next = (point + 2) % 3;
					 LIST_INSERT_TAIL(&env_sched_list[next], e, env_sched_link);
				 }
				
                 //LIST_INSERT_TAIL(&env_sched_list[1 - point], e, env_sched_link);
             }
         }
         while (1) {
             while(LIST_EMPTY(&env_sched_list[point])) {
                // point = 1 - point;
				 point = (point + 1) % 3;
             }
             e = LIST_FIRST(&env_sched_list[point]);
             if (e->env_status == ENV_FREE) {
                 LIST_REMOVE(e, env_sched_link);
             } else if (e->env_status == ENV_NOT_RUNNABLE) {
                 LIST_REMOVE(e, env_sched_link);
				 int next = (point + 1) % 3;
                 LIST_INSERT_TAIL(&env_sched_list[next], e, env_sched_link);
             } else {
				 if (point == 0) {
					 count = e->env_pri;
				 } else if (point == 1) {
					 count = (e->env_pri) * 2;
				 } else {
					 count = (e->env_pri) * 4;
                 //count = e->env_pri;
                 //break;
				 }
				 break;
             }
         }
     }
     count--;
     e->env_runs++;
	 printf("\n");
	// printf("\n%d\n",e->env_id);
     env_run(e);
	// printf("\n");
}
