#include "../drivers/gxconsole/dev_cons.h"
#include <mmu.h>
#include <env.h>
#include <printf.h>
#include <pmap.h>
#include <sched.h>

extern char *KERNEL_SP;
extern struct Env *curenv;

/* Overview:
 * 	This function is used to print a character on screen.
 * 
 * Pre-Condition:
 * 	`c` is the character you want to print.
 */
void sys_putchar(int sysno, int c, int a2, int a3, int a4, int a5)
{
	printcharc((char) c);
	return ;
}

/* Overview:
 * 	This function enables you to copy content of `srcaddr` to `destaddr`.
 *
 * Pre-Condition:
 * 	`destaddr` and `srcaddr` can't be NULL. Also, the `srcaddr` area 
 * 	shouldn't overlap the `destaddr`, otherwise the behavior of this 
 * 	function is undefined.
 *
 * Post-Condition:
 * 	the content of `destaddr` area(from `destaddr` to `destaddr`+`len`) will
 * be same as that of `srcaddr` area.
 */
void *memcpy(void *destaddr, void const *srcaddr, u_int len)
{
	char *dest = destaddr;
	char const *src = srcaddr;

	while (len-- > 0) {
		*dest++ = *src++;
	}

	return destaddr;
}

/* Overview:
 *	This function provides the environment id of current process.
 *
 * Post-Condition:
 * 	return the current environment id
 */
u_int sys_getenvid(void)
{
	return curenv->env_id;
}

/* Overview:
 *	This function enables the current process to give up CPU.
 *
 * Post-Condition:
 * 	Deschedule current environment. This function will never return.
 */
/*** exercise 4.6 ***/
void sys_yield(void)
{
	bcopy((void *)KERNEL_SP - sizeof(struct Trapframe),
        (void *)TIMESTACK - sizeof(struct Trapframe),
        sizeof(struct Trapframe));
    sched_yield();
}

/* Overview:
 * 	This function is used to destroy the current environment.
 *
 * Pre-Condition:
 * 	The parameter `envid` must be the environment id of a 
 * process, which is either a child of the caller of this function 
 * or the caller itself.
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 when error occurs.
 */
int sys_env_destroy(int sysno, u_int envid)
{
	/*
		printf("[%08x] exiting gracefully\n", curenv->env_id);
		env_destroy(curenv);
	*/
	int r;
	struct Env *e;

	if ((r = envid2env(envid, &e, 1)) < 0) {
		return r;
	}

	printf("[%08x] destroying %08x\n", curenv->env_id, e->env_id);
	env_destroy(e);
	return 0;
}

/* Overview:
 * 	Set envid's pagefault handler entry point and exception stack.
 * 
 * Pre-Condition:
 * 	xstacktop points one byte past exception stack.
 *
 * Post-Condition:
 * 	The envid's pagefault handler will be set to `func` and its
 * 	exception stack will be set to `xstacktop`.
 * 	Returns 0 on success, < 0 on error.
 */
/*** exercise 4.12 ***/
int sys_set_pgfault_handler(int sysno, u_int envid, u_int func, u_int xstacktop)
{
	// Your code here.
	struct Env *env;
	int ret;
    //printf("set pgfault handler\n");
	ret = envid2env(envid, &env, 0);
	if (ret != 0)
		return ret;
	env->env_pgfault_handler = func;
   env->env_xstacktop = xstacktop;

	return 0;
	//	panic("sys_set_pgfault_handler not implemented");
}

/* Overview:
 * 	Allocate a page of memory and map it at 'va' with permission
 * 'perm' in the address space of 'envid'.
 *
 * 	If a page is already mapped at 'va', that page is unmapped as a
 * side-effect.
 * 
 * Pre-Condition:
 * perm -- PTE_V is required,
 *         PTE_COW is not allowed(return -E_INVAL),
 *         other bits are optional.
 *
 * Post-Condition:
 * Return 0 on success, < 0 on error
 *	- va must be < UTOP
 *	- env may modify its own address space or the address space of its children
 */
/*** exercise 4.3 ***/
int sys_mem_alloc(int sysno, u_int envid, u_int va, u_int perm)
{
	// Your code here.
	struct Env *env;
	struct Page *ppage;
	int ret;
	ret = 0;
    //printf("mem alloc\n");
	if (va >= UTOP)
        return -E_INVAL;
		//panic("va >= UTOP at sys_mem_alloc\n");
	if (!(perm & PTE_V) || (perm & PTE_COW))
		return -E_INVAL;
	ret = page_alloc(&ppage);
	if (ret != 0) 
		return ret;
	ret = envid2env(envid, &env, 0);
	if (ret != 0) 
		return ret;
	ret = page_insert(env->env_pgdir, ppage, va, perm);
	if (ret != 0) 
		return ret;
	return 0;
}

/* Overview:
 * 	Map the page of memory at 'srcva' in srcid's address space
 * at 'dstva' in dstid's address space with permission 'perm'.
 * Perm has the same restrictions as in sys_mem_alloc.
 * (Probably we should add a restriction that you can't go from
 * non-writable to writable?)
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Note:
 * 	Cannot access pages above UTOP.
 */
/*** exercise 4.4 ***/
int sys_mem_map(int sysno, u_int srcid, u_int srcva, u_int dstid, u_int dstva,
				u_int perm)
{
	int ret;
	u_int round_srcva, round_dstva;
	struct Env *srcenv;
	struct Env *dstenv;
	struct Page *ppage;
	Pte *ppte;

	ppage = NULL;
	ret = 0;
	round_srcva = ROUNDDOWN(srcva, BY2PG);
	round_dstva = ROUNDDOWN(dstva, BY2PG);
    //printf("mem map %x %x\n", srcva, dstva);
    //your code here
	if (dstva >= UTOP || srcva >= UTOP)
        return -E_INVAL;
		//panic("dstva >= UTOP at sys_mem_map\n");
	if ((perm & PTE_V) == 0)
		return -E_INVAL;
	ret = envid2env(srcid, &srcenv, 0);
	if (ret != 0) 
		return ret;
	ret = envid2env(dstid, &dstenv, 0);
	if (ret != 0) 
		return ret;
	ppage = page_lookup(srcenv->env_pgdir, round_srcva, &ppte);
	ret = page_insert(dstenv->env_pgdir, ppage, round_dstva, perm);

	return ret;
}

/* Overview:
 * 	Unmap the page of memory at 'va' in the address space of 'envid'
 * (if no page is mapped, the function silently succeeds)
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Cannot unmap pages above UTOP.
 */
/*** exercise 4.5 ***/
int sys_mem_unmap(int sysno, u_int envid, u_int va)
{
	// Your code here.
	int ret;
	struct Env *env;
    //printf("mem unmap\n");
	if (va >= UTOP)
        return -E_INVAL;
		//panic("va >= UTOP at sys_mem_unmap\n");
	ret = envid2env(envid, &env, 0);
	if (ret != 0)
		return ret;
	page_remove(env->env_pgdir, va);

	return ret;
	//	panic("sys_mem_unmap not implemented");
}

/* Overview:
 * 	Allocate a new environment.
 *
 * Pre-Condition:
 * The new child is left as env_alloc created it, except that
 * status is set to ENV_NOT_RUNNABLE and the register set is copied
 * from the current environment.
 *
 * Post-Condition:
 * 	In the child, the register set is tweaked so sys_env_alloc returns 0.
 * 	Returns envid of new environment, or < 0 on error.
 */
/*** exercise 4.8 ***/
int sys_env_alloc(void)
{
	// Your code here.
	int r;
	struct Env *e;
    //printf("env alloc\n");
	r = env_alloc(&e, curenv->env_id);
	if (r != 0)
		return r;
	e->env_status = ENV_NOT_RUNNABLE;
	e->env_pri = curenv->env_pri;
	//e->env_cr3 = curenv->env_cr3;
	bcopy((void*)KERNEL_SP - sizeof(struct Trapframe),
              (void*)&(e->env_tf),
              sizeof(struct Trapframe));
	e->env_tf.pc = e->env_tf.cp0_epc;
	e->env_tf.regs[2] = 0; //set return($v0) as 0

	return e->env_id;
	//	panic("sys_env_alloc not implemented");
}

/* Overview:
 * 	Set envid's env_status to status.
 *
 * Pre-Condition:
 * 	status should be one of `ENV_RUNNABLE`, `ENV_NOT_RUNNABLE` and
 * `ENV_FREE`. Otherwise return -E_INVAL.
 * 
 * Post-Condition:
 * 	Returns 0 on success, < 0 on error.
 * 	Return -E_INVAL if status is not a valid status for an environment.
 * 	The status of environment will be set to `status` on success.
 */
/*** exercise 4.14 ***/
int sys_set_env_status(int sysno, u_int envid, u_int status)
{
	// Your code here.
	struct Env *env;
	int ret;
    //printf("set env status\n");
	ret = envid2env(envid, &env, 0);
	if (ret != 0)
		panic("set env status inviable id\n");
	if (status != ENV_RUNNABLE && status != ENV_FREE && status != ENV_NOT_RUNNABLE)
		return -E_INVAL;
	env->env_status = status;
	LIST_INSERT_TAIL(env_sched_list, env, env_sched_link);

	return 0;
	//	panic("sys_env_set_status not implemented");
}

/* Overview:
 * 	Set envid's trap frame to tf.
 *
 * Pre-Condition:
 * 	`tf` should be valid.
 *
 * Post-Condition:
 * 	Returns 0 on success, < 0 on error.
 * 	Return -E_INVAL if the environment cannot be manipulated.
 *
 * Note: This hasn't be used now?
 */
int sys_set_trapframe(int sysno, u_int envid, struct Trapframe *tf)
{

	return 0;
}

/* Overview:
 * 	Kernel panic with message `msg`. 
 *
 * Pre-Condition:
 * 	msg can't be NULL
 *
 * Post-Condition:
 * 	This function will make the whole system stop.
 */
void sys_panic(int sysno, char *msg)
{
	// no page_fault_mode -- we are trying to panic!
	panic("%s", TRUP(msg));
}

/* Overview:
 * 	This function enables caller to receive message from 
 * other process. To be more specific, it will flag 
 * the current process so that other process could send 
 * message to it.
 *
 * Pre-Condition:
 * 	`dstva` is valid (Note: NULL is also a valid value for `dstva`).
 * 
 * Post-Condition:
 * 	This syscall will set the current process's status to 
 * ENV_NOT_RUNNABLE, giving up cpu. 
 */
/*** exercise 4.7 ***/
void sys_ipc_recv(int sysno, u_int dstva)
{
   if (dstva >= UTOP)
		return;
	curenv->env_ipc_recving = 1;
	curenv->env_ipc_dstva = dstva;
	curenv->env_status = ENV_NOT_RUNNABLE;
	sys_yield();
}

/* Overview:
 * 	Try to send 'value' to the target env 'envid'.
 *
 * 	The send fails with a return value of -E_IPC_NOT_RECV if the
 * target has not requested IPC with sys_ipc_recv.
 * 	Otherwise, the send succeeds, and the target's ipc fields are
 * updated as follows:
 *    env_ipc_recving is set to 0 to block future sends
 *    env_ipc_from is set to the sending envid
 *    env_ipc_value is set to the 'value' parameter
 * 	The target environment is marked runnable again.
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Hint: the only function you need to call is envid2env.
 */
/*** exercise 4.7 ***/
int sys_ipc_can_send(int sysno, u_int envid, u_int value, u_int srcva,
					 u_int perm)
{

	int r;
	struct Env *e;
	struct Page *p;
    //printf("ipc can send\n");
	r = envid2env(envid, &e, 0);
	if (r != 0)
		return r;
	if (e->env_ipc_recving != 1)
		return -E_IPC_NOT_RECV;
	if (srcva != 0)
	{
		r = sys_mem_map(sysno, curenv->env_id, srcva, envid, e->env_ipc_dstva, perm);
		if (r != 0)
			return r;
	}
	e->env_ipc_recving = 0;
	e->env_ipc_from = curenv->env_id;
	e->env_ipc_value = value;
	e->env_ipc_perm = perm;
	e->env_status = ENV_RUNNABLE;

	return 0;
}

// lab6-extra
/*int
sys_init_PV_var(int sys, int init_value)
{
	struct Pv *p;
	int pvid = get_new_pv(&p);
	p->pv_id = pvid;
	p->semaphore = init_value;
	p->waiting_first = 0;
	p->waiting_last = 0;
	return pvid;
}

void
sys_P(int sys, int pv_id)
{
	struct Pv *p = 0;
	int found = 0;
	found = pvid2pv(pv_id, &p);
	if (found == -1)
		return;
   // printf("%d p %x\n", curenv->env_id,p);
	p->semaphore--;
	if (p->semaphore < 0)
	{
		curenv->env_status = ENV_NOT_RUNNABLE;
      //  printf("sleep %d\n", curenv->env_id);
		(p->waitingID)[p->waiting_last] = curenv->env_id;
		p->waiting_last = (p->waiting_last++) % PVWAITNUM;
		sched_yield();
	}
}

void 
sys_V(int sys, int pv_id)
{
	struct Pv *p = 0;
	int found = 0;
	found = pvid2pv(pv_id, &p);
	if (found == -1)
		return;
    //printf("%d v %x\n",curenv->env_id, p);
	p->semaphore++;
	if (p->semaphore <= 0 && p->waiting_first != p->waiting_last)
	{
		struct Env *e;
		u_int id = ((p->waitingID)[p->waiting_first]);
		envid2env(id, &e, 0);
       // printf("awake %d\n", e->env_id);
		e->env_status = ENV_RUNNABLE;
		p->waiting_first = (p->waiting_first++) % PVWAITNUM;
	}
}

int
sys_check_PV_value(int sys, int pv_id)
{
	struct Pv *p = 0;
	int found = 0;
	found = pvid2pv(pv_id, &p);
	if (found == -1)
		return -1;
	return p->semaphore;
}

void 
sys_release_PV_var(int sys, int pv_id)
{
	struct Pv *p = 0;
	int found = 0;
	found = pvid2pv(pv_id, &p);
	if (found == -1)
		return;
	int i = p->waiting_first;
	while(p->waiting_last != i)
	{
		struct Env *e;
		u_int id = ((p->waitingID)[i]);
		envid2env(id, &e, 0);
		e->env_status = ENV_FREE;
		i = (i++) % PVWAITNUM;
	}
}*/
int
sys_init_PV_var(int sysno,int init_value){
	struct PV *p;
	int r = mkPVid();
	if(r > 1024){
		return -1;
	}
	int rr;
	if((rr = PVid2PV(r,&p))!=0){
		return -1;
	}
//	printf("%d\n",r);
	p->PV_id = r;
	p->PV_alloc = 1;
	p->PV_value = init_value;
	LIST_INIT(&(p->PV_wait_list));
	return p->PV_id;
}

void
sys_P(int sysno,int pv_id){
	struct PV *p;
	int r;
	if((r = PVid2PV(pv_id,&p))!=0){
		return;
	}
	if(p->PV_alloc == 0){
		return;
	}
//	printf("syscall_P:success\n");
	p->PV_value--;
	if(p->PV_value<0){
		curenv->env_status = ENV_NOT_RUNNABLE;
		LIST_INSERT_TAIL(&(p->PV_wait_list),curenv,PV_wait_link);
		sys_yield();
	}
}
void
sys_V(int sysno,int pv_id){
	struct PV *p;
	int r;
	if((r = PVid2PV(pv_id,&p))!=0){
		return;
	}
	if(p->PV_alloc ==0){
		return;
	}
//	printf("syscall_V:success\n");
	p->PV_value++;
	if(p->PV_value <= 0){
		struct Env *e;
		e = LIST_FIRST(&(p->PV_wait_list));
		e->env_status = ENV_RUNNABLE;
		LIST_REMOVE(e,PV_wait_link);
		sys_yield();
	}
}
int 
sys_check_PV_value(int sysno,int pv_id){
	struct PV *p;
	int r;
	if((r =PVid2PV(pv_id,&p))!=0){
		return -1;
	}
	if(p->PV_alloc == 0){
		return -1;
	}
	return p->PV_value;
}
void
sys_release_PV_var(int sysno,int pv_id){
	struct PV *p;
	int r;
	if((r = PVid2PV(pv_id,&p))!=0){
		return;
	}
	p->PV_alloc = 0;
	struct Env *e;
	LIST_FOREACH((e),&(p->PV_wait_list),PV_wait_link){
		env_free(e);
//		sys_yield();
	}
}
