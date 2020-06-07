// Ping-pong a counter between two processes.
// Only need to start one of these -- splits into two with fork.

#include "lib.h"

void
umain(void)
{
	u_int empty, full, mutex;

	/*if ((who = fork()) != 0) {
		// get the ball rolling
		writef("\n@@@@@send 0 from %x to %x\n", syscall_getenvid(), who);
		ipc_send(who, 0, 0, 0);
		//user_panic("&&&&&&&&&&&&&&&&&&&&&&&&m");
	}

	for (;;) {
		writef("%x am waiting.....\n",syscall_getenvid());
		i = ipc_recv(&who, 0, 0);
		
		writef("%x got %d from %x\n", syscall_getenvid(), i, who);
	
		//user_panic("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
		if (i == 10)
			return;
		i++;
		writef("\n@@@@@send 0 from %x to %x\n", syscall_getenvid(), who);
		ipc_send(who, i, 0, 0);
		if (i == 10)
			return;
	}*/
	empty = syscall_init_PV_var(2);
	full = syscall_init_PV_var(0);
	mutex = syscall_init_PV_var(1);
	writef("empty: %d;full: %d;mutex: %d\n", empty, full, mutex);

 if (fork() == 0) {
  int i = 0;
     writef("child begin\n");
  for (i = 0; i < 10; i++) {
   syscall_P(empty);
   syscall_P(mutex);
   writef("produce %d\n", i);
   syscall_V(mutex);
   syscall_V(full);
  }
  syscall_release_PV_var(full);
  //syscall_P(full);
 } else {
  int j = 0;
     //writef("father begin\n");
  for (j = 0; j < 10; j++) {
      writef("mutex %x\n", &mutex);
   syscall_P(full);
      //writef("father back\n");
      writef("mutex %x\n", &mutex);
   syscall_P(mutex);
      writef("checking\n");
      //user_panic("???\n");
      syscall_check_PV_value(mutex);
      writef("checked\n");
   writef("mutex value is %d\n", syscall_check_PV_value(mutex));
   writef("consume %d\n", j);
   syscall_V(mutex);
   syscall_V(empty);
  }
  syscall_P(full);
  //syscall_release_PV_var(full);
 }
}

