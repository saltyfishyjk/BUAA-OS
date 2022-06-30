/* See COPYRIGHT for copyright information. */

#ifndef _ENV_H_
#define _ENV_H_

#include "types.h"
#include "queue.h"
#include "trap.h"
#include "mmu.h" 

#define LOG2NENV	10
#define NENV		(1<<LOG2NENV)
#define ENVX(envid)	((envid) & (NENV - 1))
#define GET_ENV_ASID(envid) (((envid)>> 11)<<6)

// Values of env_status in struct Env
#define ENV_FREE	0
#define ENV_RUNNABLE		1
#define ENV_NOT_RUNNABLE	2

/* Lab4-challenge alter begin */
/* define values */
#define MAX_NAME 20
#define MAXN 50

/* thread types */
typedef u_int pthread_t; // thread id, equals mkenvid in MOS
typedef u_int pthread_key_t; // thread private key value
/* thread attribute */
typedef struct{
	u_int detachstate; // thread detach state
	u_int stacksize; // thread stack size
}pthread_attr_t;
/* semaphore types */
/* semaphore */
typedef struct{
	int val; // sem value 
	int cnt; // sem cnt. represents how many threads can block on it
}sem_t;
/* named sem, we store some of it */
struct Named_Sem{
	char name[MAX_NAME + 1];
	sem_t sem;
};
/* thread key data structure */
struct Key{
	int tag; // flag if the position is used
	void (*destructor)(void *); // destruct function
};
/* Clean up function structure */
struct Cleanup{
	void (*rtn)(void *); // function address
	void *arg; // arguments
};

#define E_CANCEL_STATE_DISABLE 13 // thread cancel state is DISABLE
#define E_NO_THREAD 14 // no relevant thread
#define E_IS_DETACHED 15 // relevant thread is detached
#define E_SEM 16 // init sem
#define ENV_ZOMBIE 3 // zombie thread

#define PTHREAD_CANCEL_DISABLE 0 // thread cancel state is DISABLE
#define PTHREAD_CANCEL_ENABLE 1 // thread cancel state is ENABLE

#define PTHREAD_CANCEL_ASYNCHRONOUS 0 // thread cancel state is asynchronous
#define PTHREAD_CANCEL_DEFERRED 1 // thread cancel state is deferred

#define PTHREAD_CREATE_JOINABLE 0 // thread is not joined
#define PTHREAD_CREATE_DETACHED 1 // thread is detached 

//#define MAX_NAME 20
//#define MAXN 50
/* Lab4-challenge alter end */

struct Env {
	struct Trapframe env_tf;        // Saved registers
	LIST_ENTRY(Env) env_link;       // Free list
	u_int env_id;                   // Unique environment identifier
	u_int env_parent_id;            // env_id of this env's parent
	u_int env_status;               // Status of the environment
	Pde  *env_pgdir;                // Kernel virtual address of page dir
	u_int env_cr3;
	LIST_ENTRY(Env) env_sched_link;
        u_int env_pri;
	// Lab 4 IPC
	u_int env_ipc_value;            // data value sent to us 
	u_int env_ipc_from;             // envid of the sender  
	u_int env_ipc_recving;          // env is blocked receiving
	u_int env_ipc_dstva;		// va at which to map received page
	u_int env_ipc_perm;		// perm of page mapping received

	// Lab 4 fault handling
	u_int env_pgfault_handler;      // page fault state
	u_int env_xstacktop;            // top of exception stack

	// Lab 6 scheduler counts
	u_int env_runs;			// number of times been env_run'ed
	u_int env_nop;                  // align to avoid mul instruction
	
	/* Lab4-challenge altered begin */
	/* thread related */
	u_int thread_parent; // main thread id
	u_int stacktop; // thread stack top
	
	u_int cancel_state; // thread cancel state
	u_int cancel_type; // thread cancel type
	u_int is_cancled; // if reveived cancel request from other thread
	pthread_attr_t attr; // thread attribute
	u_int is_joined; // if being waited to end by other threads
	void *value; // value passed when THIS thread uses pthread_exit
	void **ptr; // ptr passed by other thread when using pthread_join
	
	void *key_value[MAXN]; // Thread Specific Data
	struct Cleanup sk[MAXN]; // clean up function stack 
	int top; // thread clean up function stack top
	
	void (*prepare)(void); // address of prepare function in atfork
	void (*parent)(void); // address of parent function in atfork
	void (*child)(void); // address of child function in atfork
};

LIST_HEAD(Env_list, Env);
extern struct Env *envs;		// All environments
extern struct Env *curenv;	        // the current env
extern struct Env_list env_sched_list[2]; // runnable env list

void env_init(void);
int env_alloc(struct Env **e, u_int parent_id);
void env_free(struct Env *);
void env_create_priority(u_char *binary, int size, int priority);
void env_create(u_char *binary, int size);
void env_destroy(struct Env *e);

int envid2env(u_int envid, struct Env **penv, int checkperm);
void env_run(struct Env *e);


// for the grading script
#define ENV_CREATE2(x, y) \
{ \
	extern u_char x[], y[]; \
	env_create(x, (int)y); \
}
#define ENV_CREATE_PRIORITY(x, y) \
{\
        extern u_char binary_##x##_start[]; \
        extern u_int binary_##x##_size;\
        env_create_priority(binary_##x##_start, \
                (u_int)binary_##x##_size, y);\
}
#define ENV_CREATE(x) \
{ \
	extern u_char binary_##x##_start[];\
	extern u_int binary_##x##_size; \
	env_create(binary_##x##_start, \
		(u_int)binary_##x##_size); \
}

#endif // !_ENV_H_
