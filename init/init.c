#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>

/*void mips_init()
{
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();

	mips_vm_init();
	page_init();

	env_init();
	env_check();
	load_icode_check();*/

	/*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
	 * code_b.c*/
	/*** exercise 3.9 ***/
	/*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
	 * interesting, have fun please*/
	/*ENV_CREATE_PRIORITY(user_A, 2);
	ENV_CREATE_PRIORITY(user_B, 1);


	//trap_init();
	//kclock_init();
	panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of mips_init() reached!");
}*/

/* alter for lab3-1-exam*/
void lab3_1_exam_test(){
	struct Env *envs[100];
    struct Env *e;
    int i;
    
    /* alloc 100 envs for testing */
    for (i = 0; i < 100; i++) {
        if (env_alloc(&e, 0) != 0) {
            return;
        }
        envs[i] = e;
    }
    
    /* run 64 env to use all asid */
    for (i = 0; i < 64; i++) {
        exam_env_run(envs[i]);
    }
    
    /* run env with valid asid, should not flush TLB */
    printf("ret = %d\n", exam_env_run(envs[10]));
    
    /* first free a env, so we get a free asid */
    exam_env_free(envs[0]);
    envs[0]->env_asid = 0;
    
    /* run a env, it should have hardware asid of 0 */
    printf("ret = %d\n", exam_env_run(envs[64]));
    printf("envid = %x, asid = %x\n", envs[64]->env_id, envs[64]->env_asid);
    
    /* run a env, there should be no free asid, so we should flush TLB */
    printf("ret = %d\n", exam_env_run(envs[0]));
    printf("envid = %x, asid = %x\n", envs[0]->env_id, envs[0]->env_asid);
    
    /* run a env with invalid asid, but its hardware asid is free, so only update its generation */
    printf("ret = %d\n", exam_env_run(envs[5]));
    printf("envid = %x, asid = %x\n", envs[5]->env_id, envs[5]->env_asid);
    
    /* run a env with invalid asid, and its hardware asid is not free, so alloc a new asid */
    printf("ret = %d\n", exam_env_run(envs[64]));
    printf("envid = %x, asid = %x\n", envs[64]->env_id, envs[64]->env_asid);
}

void mips_init(){
    mips_detect_memory();
    mips_vm_init();
    page_init();
    
    env_init();
    
    lab3_1_exam_test();
    
    *((volatile char*)(0xB0000010)) = 0;
}

/* alter for lab3-1-exam finished*/

void bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	max = dst + len;
	// copy machine words while possible
	while (dst + 3 < max)
	{
		*(int *)dst = *(int *)src;
		dst+=4;
		src+=4;
	}
	// finish remaining 0-3 bytes
	while (dst < max)
	{
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
}

void bzero(void *b, size_t len)
{
	void *max;

	max = b + len;

	//printf("init.c:\tzero from %x to %x\n",(int)b,(int)max);

	// zero machine words while possible

	while (b + 3 < max)
	{
		*(int *)b = 0;
		b+=4;
	}

	// finish remaining 0-3 bytes
	while (b < max)
	{
		*(char *)b++ = 0;
	}

}
