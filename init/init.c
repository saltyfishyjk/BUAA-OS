#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>

extern char aoutcode[];
extern char boutcode[];

void mips_init()
{
	printf("init.c:\tmips_init() is called\n");
	printf("env is %d\n",sizeof(struct Env));
	printf("Tcb is %d\n",sizeof(struct Tcb));
	printf("Trapframe is %d\n",sizeof(struct Trapframe));
	mips_detect_memory();
	
	mips_vm_init();
	page_init();
	
	env_init();
	
	//ENV_CREATE(user_fktest);
	//ENV_CREATE(user_pingpong);
	//ENV_CREATE_PRIORITY(user_a,1);
	//ENV_CREATE_PRIORITY(user_b,2);
	//ENV_CREATE(user_helloworld);
	//ENV_CREATE(user_pttest);
	//ENV_CREATE(user_ptstacktest);
	//ENV_CREATE(user_ptexittest);
	//ENV_CREATE(user_ptcanceltest);
	//ENV_CREATE(user_ptjointest);
	//ENV_CREATE(user_semtest);
	ENV_CREATE(user_seller);
	
    trap_init();
	kclock_init();

	
	while(1);
	panic("init.c:\tend of mips_init() reached!");
}

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
