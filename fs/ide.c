/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

#define IDE_BEGIN_ADDR 0x13000000
#define IDE_OFFSET_ADDR IDE_BEGIN_ADDR + 0x0000
#define IDE_OFFSETHI_ADDR IDE_BEGIN_ADDR + 0x0008
#define IDE_ID_ADDR IDE_BEGIN_ADDR + 0x0010
#define IDE_OP_ADDR IDE_BEGIN_ADDR + 0x0020
#define IDE_STATUS_ADDR IDE_BEGIN_ADDR + 0x0030
#define IDE_BUFFER_ADDR IDE_BEGIN_ADDR + 0x4000
#define IDE_BUFFER_SIZE 0x0200

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	while (offset_begin + offset < offset_end) {
            // Your code here
            // error occurred, then panic.
		// write the offset address
		u_int tmp = offset + offset_begin;
		if (syscall_write_dev(&tmp, IDE_OFFSET_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-read %X fail 1\n", offset + offset_begin);
		// write the disk number
		if (syscall_write_dev(&diskno, IDE_ID_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-read %X fail 2\n", offset + offset_begin);
		// start to read
		tmp = 0;
		if (syscall_write_dev(&tmp, IDE_OP_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-read %X fail 3\n", offset + offset_begin);
		// check if read success
		if (syscall_read_dev(&tmp, IDE_STATUS_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-read %X fail 4\n", offset + offset_begin);
		if (tmp == 0)
			user_panic("ide-read %X fail 5\n", offset + offset_begin);
		// if success, read from buffer
		if (syscall_read_dev(dst + offset, IDE_BUFFER_ADDR, IDE_BUFFER_SIZE) != 0)
			user_panic("ide-read %X fail 6\n", offset + offset_begin);
		// next loop
		offset += IDE_BUFFER_SIZE;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
	int offset_begin = secno * IDE_BUFFER_SIZE;
	int offset_end = offset_begin + nsecs * IDE_BUFFER_SIZE;
	int offset = 0;

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
	    // copy data from source array to disk buffer.

            // if error occur, then panic.
		// write the offset address
		u_int tmp = offset + offset_begin;
		if (syscall_write_dev(&tmp, IDE_OFFSET_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-write %X fail 1\n", offset + offset_begin);
		// write the disk number
		if (syscall_write_dev(&diskno, IDE_ID_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-write %X fail 2\n", offset + offset_begin);
		// write the src to buffer
		if (syscall_write_dev(src + offset, IDE_BUFFER_ADDR, IDE_BUFFER_SIZE) != 0)
			user_panic("ide-write %X fail 6\n", offset + offset_begin);
		// begin to write
		tmp = 1;
		if (syscall_write_dev(&tmp, IDE_OP_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-write %X fail 3\n", offset + offset_begin);
		// check if write success
		if (syscall_read_dev(&tmp, IDE_STATUS_ADDR, sizeof(u_int)) != 0)
			user_panic("ide-write %X fail 4\n", offset + offset_begin);
		if (tmp == 0)
			user_panic("ide-write %X fail 5\n", offset + offset_begin);
		// next loop
		offset += IDE_BUFFER_SIZE;
	}
}

