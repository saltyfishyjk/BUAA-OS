// implement fork from user space

#include "lib.h"
#include <mmu.h>
#include <env.h>


/* ----------------- help functions ---------------- */

/* Overview:
 * 	Copy `len` bytes from `src` to `dst`.
 *
 * Pre-Condition:
 * 	`src` and `dst` can't be NULL. Also, the `src` area 
 * 	 shouldn't overlap the `dest`, otherwise the behavior of this 
 * 	 function is undefined.
 */
void user_bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	//	writef("~~~~~~~~~~~~~~~~ src:%x dst:%x len:%x\n",(int)src,(int)dst,len);
	max = dst + len;

	// copy machine words while possible
	if (((int)src % 4 == 0) && ((int)dst % 4 == 0)) {
		while (dst + 3 < max) {
			*(int *)dst = *(int *)src;
			dst += 4;
			src += 4;
		}
	}

	// finish remaining 0-3 bytes
	while (dst < max) {
		*(char *)dst = *(char *)src;
		dst += 1;
		src += 1;
	}

	//for(;;);
}

/* Overview:
 * 	Sets the first n bytes of the block of memory 
 * pointed by `v` to zero.
 * 
 * Pre-Condition:
 * 	`v` must be valid.
 *
 * Post-Condition:
 * 	the content of the space(from `v` to `v`+ n) 
 * will be set to zero.
 */
void user_bzero(void *v, u_int n)
{
	char *p;
	int m;

	p = v;
	m = n;

	while (--m >= 0) {
		*p++ = 0;
	}
}
/*--------------------------------------------------------------*/

/* Overview:
 * 	Custom page fault handler - if faulting page is copy-on-write,
 * map in our own private writable copy.
 * 
 * Pre-Condition:
 * 	`va` is the address which leads to a TLBS exception.
 *
 * Post-Condition:
 *  Launch a user_panic if `va` is not a copy-on-write page.
 * Otherwise, this handler should map a private writable copy of 
 * the faulting page at correct address.
 */
static void
pgfault(u_int va)
{
	va = ROUNDDOWN(va,BY2PG);
	u_int tmp = USTACKTOP;
	u_int perm = (*vpt)[va/BY2PG]&0xfff&(~PTE_COW);
	//	writef("fork.c:pgfault():\t va:%x\n",va);
	if (!((*vpt)[va/BY2PG]&PTE_COW))
		user_panic("^^^^panic at pgfault^^^^perm doesn't have PTE_COW!!^^^^");
	
    
    //map the new page at a temporary place
	if (syscall_mem_alloc(0,tmp,perm|PTE_V|PTE_R) < 0)
		user_panic("^^panic at pgfault^^mem alloc error^^");

	//copy the content
	user_bcopy(va,tmp,BY2PG);
    //map the page on the appropriate place
	if (syscall_mem_map(0,tmp,0,va,perm|PTE_V|PTE_R) < 0)
		user_panic("^^panic at pgfault^^mem map error^^");
    //unmap the temporary place
	if (syscall_mem_unmap(0,tmp) < 0)
		user_panic("^^panic at pgfault^^mem unmap for tmp error^^");

	return;
	
}

/* Overview:
 * 	Map our virtual page `pn` (address pn*BY2PG) into the target `envid`
 * at the same virtual address. 
 *
 * Post-Condition:
 *  if the page is writable or copy-on-write, the new mapping must be 
 * created copy on write and then our mapping must be marked 
 * copy on write as well. In another word, both of the new mapping and
 * our mapping should be copy-on-write if the page is writable or 
 * copy-on-write.
 * 
 * Hint:
 * 	PTE_LIBRARY indicates that the page is shared between processes.
 * A page with PTE_LIBRARY may have PTE_R at the same time. You
 * should process it correctly.
 */
static void
duppage(u_int envid, u_int pn)
{
	u_int addr = pn*BY2PG;
	u_int perm = (*vpt)[pn] & 0xfff;
	int r;

	if (((*vpt)[pn]&PTE_R)) {
		if ((*vpt)[pn]&PTE_LIBRARY) {
			//writef("to1\n");
			r = syscall_mem_map(0,addr,envid,addr,perm|PTE_V|PTE_R|PTE_LIBRARY);
			//writef("1\n");
			if (r < 0)
				return;	
			//writef("to2\n");
			r = syscall_mem_map(0,addr,0,addr,perm|PTE_V|PTE_R|PTE_LIBRARY);
			//writef("2\n");
			if (r < 0)
				return;		
		} else {
			//writef("to3\n");
			//writef("0x%x\n",addr);
			r = syscall_mem_map(0,addr,envid,addr,perm|PTE_V|PTE_R|PTE_COW);
			//writef("3\n");
			if (r < 0)
				return;
			//writef("to4\n");
			r = syscall_mem_map(0,addr,0,addr,perm|PTE_V|PTE_R|PTE_COW);
			//writef("4\n");
			if (r < 0)
				return;
		}
	} else {
		//writef("to5\n");
		r = syscall_mem_map(0,addr,envid,addr,perm|PTE_V);
		//writef("5\n");
		if (r < 0)
			return;
	}

	//	user_panic("duppage not implemented");
}

/* Overview:
 * 	User-level fork. Create a child and then copy our address space
 * and page fault handler setup to the child.
 *
 * Hint: use vpd, vpt, and duppage.
 * Hint: remember to fix "env" in the child process!
 * Note: `set_pgfault_handler`(user/pgfault.c) is different from 
 *       `syscall_set_pgfault_handler`. 
 */
extern void __asm_pgfault_handler(void);
int
fork(void)
{
	// Your code here.
	u_int newenvid;
	extern struct Env *envs;
	extern struct Env *env;
	u_int i;
	u_int pn;
	u_int j;


	//The parent installs pgfault using set_pgfault_handler
	//writef("vpt is 0x%x\n",vpt);
	//alloc a new alloc
	set_pgfault_handler(pgfault);	
	newenvid = syscall_env_alloc();
	if (newenvid == 0) {
		env = &envs[ENVX(syscall_getenvid())];
		tcb = &env->env_threads[0];
		return 0;
	}
	if (newenvid != 0) {
		for (i = 0 ; i <= PDX(USTACKTOP); i++) {
			if ((*vpd)[i]&PTE_V) {
				pn = i*1024;
				for (j = 0; j < 1024; j++) {
					if (((pn+j)*BY2PG) >= USTACKTOP)
						break;
					if ((*vpt)[pn+j]&PTE_V) {
						duppage(newenvid,pn+j);
					}
				}
			}
		}
		/*for (i = 0; i < USTACKTOP; i += BY2PG) {
			if ((*vpd)[PDX(i)]&PTE_V) {
				if ((*vpt)[i/BY2PG]&PTE_V) {
					duppage(newenvid,i/BY2PG);
				}
			}
		}*/
		if ((i = syscall_mem_alloc(newenvid,UXSTACKTOP - BY2PG, PTE_V|PTE_R)) < 0)
			return newenvid;
		if ((i = syscall_set_pgfault_handler(newenvid,__asm_pgfault_handler, UXSTACKTOP)) < 0)
			return newenvid;
		if ((i = syscall_set_env_status(newenvid,ENV_RUNNABLE)) < 0)
			return newenvid;
	}
	return newenvid;
}

// Challenge!
int
sfork(void)
{
	user_panic("sfork not implemented");
	return -E_INVAL;

}
