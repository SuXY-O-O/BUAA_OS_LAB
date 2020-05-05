#include "lib.h"

void
umain(void)
{
    u_int a = 0x800;
    u_int c = 0x1802;
    u_int m = 0;
    u_int n = 0;
    while (1)
    {
        ipc_send_double(a, c, m, 0, 0);
        writef("%x is waiting\n", syscall_getenvid());
        m = ipc_recv(&a, 0, 0);
        n = ipc_recv(&c, 0, 0);
        writef("%x : m : %d : n : %d\n", syscall_getenvid(), m, n);
        if (m == 5)
            break;
        m += 1;
    }
}