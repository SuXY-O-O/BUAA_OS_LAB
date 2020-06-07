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
/*** exercise 3.14 ***/
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
	struct Env *e;
	if(curenv){
		count ++;
		if(count < curenv->env_pri  && curenv->env_status == ENV_RUNNABLE){
			env_run(curenv);
			return;
		}else{
			LIST_REMOVE(curenv,env_sched_link);
			LIST_INSERT_TAIL(&env_sched_list[1-point],(curenv),env_sched_link);
			count = 0;
		}
	}
	count = 0;
	while(1){
		if(LIST_EMPTY(&env_sched_list[point])){
			point = 1 - point;
		}
		if((e = LIST_FIRST(&(env_sched_list[point])))== NULL){
			continue;
		}
		if(e->env_status == ENV_FREE){
			LIST_REMOVE((e),env_sched_link);
		} else if(e->env_status == ENV_NOT_RUNNABLE){
			LIST_REMOVE((e),env_sched_link);
			LIST_INSERT_TAIL(&(env_sched_list[1-point]),(e),env_sched_link);
		} else if(e -> env_status == ENV_RUNNABLE){
			env_run(e);
		}
	}
}
