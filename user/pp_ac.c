#include "lib.h"

void
umain(void)
{
    u_int b = 0x1001;
    u_int m = 0;
    while (1)
    {
        writef("%x is waiting\n", syscall_getenvid());
        m = ipc_recv(&b, 0, 0);
        writef("%x : m : %d", syscall_getenvid(), m);
        m += 1;
        ipc_send(b, m, 0, 0);
        if (m == 5)
            break;
    }
}