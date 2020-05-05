#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>

/*** exercise 4.2 ***/
NESTED(handle_sys,TF_SIZE, sp)
    SAVE_ALL                            // Macro used to save trapframe
    CLI                                 // Clean Interrupt Mask
    nop
    .set at                             // Resume use of $at

    // TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.
    lw      t0, TF_EPC(sp)
    addiu    t0, t0, 4
    sw      t0, TF_EPC(sp)
    
    // TODO: Copy the syscall number into $a0.
    lw      a0, TF_REG4(sp)   
 
    addiu   a0, a0, -__SYSCALL_BASE     // a0 <- relative syscall number
    sll     t0, a0, 2                   // t0 <- relative syscall number times 4
    la      t1, sys_call_table          // t1 <- syscall table base
    addu    t1, t1, t0                  // t1 <- table entry of specific syscall
    lw      t2, 0(t1)                   // t2 <- function entry of specific syscall

    //lab4 extra
    lw      t0, TF_REG29(sp)            // t0 <- user's stack pointer
    move    s0, sp                      // save sp to s0
    lw      a0, TF_REG4(sp)             // load syscall number
    lw      t3, TF_REG5(sp)             // t3 <- args number
    move    s1, t3                      // store args number to s1
    lw      a1, TF_REG6(sp)             // load a1
    lw      a2, TF_REG7(sp)             // load a2
    lw      a3, 16(t0)                  // load a3, the 5th argument of msyscall
    addiu   t0, t0, 20                  // t0 <- begin address of other args
    addiu   sp, sp, -16                 // space for a0 - a3
    addiu   t3, t3, -3                  // we already have 3 spaces
    FOR_BEGIN:
        blez    t3, FOR_END
        nop
        addiu   t3, t3, -1
        addiu   sp, sp, -4
        j       FOR_BEGIN
        nop
    FOR_END:                 
    sw      a0, 0(sp)                   // we will not change sp later
    sw      a1, 4(sp)
    sw      a2, 8(sp)
    sw      a3, 12(sp)
    addiu   t4, sp, 16                  // t4 <- begin address to store other args
    addiu   t3, s1, -3
    FOR_2_BEGIN:
        blez    t3, FOR_2_END
        nop
        addiu   t3, t3, -1
        lw      t5, 0(t0)
        addiu   t0, t0, 4
        sw      t5, 0(t4) 
        addiu   t4, t4, 4
        j       FOR_2_BEGIN
        nop
    FOR_2_END:
    
    jalr    t2                          // Invoke sys_* function
    nop
    
    // TODO: Resume current kernel stack
    move    sp, s0
 
    sw      v0, TF_REG2(sp)             // Store return value of function sys_* (in $v0) into trapframe

    j       ret_from_exception          // Return from exeception
    nop
END(handle_sys)

sys_call_table:                         // Syscall Table
.align 2
    .word sys_putchar
    .word sys_getenvid
    .word sys_yield
    .word sys_env_destroy
    .word sys_set_pgfault_handler
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_env_alloc
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv
    .word sys_cgetc
    .word sys_super_multi_parameter