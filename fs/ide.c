/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

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
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	u_int zero = 0;
	u_int cur_offset = 0;
	int op_status = 0;
	int read = 0;
	int can_read = 0;
	while (offset_begin + offset < offset_end) {
		// Your code here
		cur_offset = offset_begin + offset; // calc current offset
		/* set diskno */
		if (syscall_write_dev((u_int)&diskno, 0x13000010, 4) != 0) {
			user_panic("write failed!\n");
		}
		/* set offset */
		if (syscall_write_dev((u_int)&cur_offset, 0x13000000, 4) != 0) {
			user_panic("write failed!\n");
		}
		/* set value */
		if (syscall_write_dev((u_int)&read, 0x13000020, 4) != 0) {
			user_panic("write failed!\n");
		}

		if (syscall_read_dev(&op_status, 0x13000030, 4) != 0) {
			user_panic("write failed!\n");
		}
		if (op_status == 0) {
			user_panic("read failed!\n");
		}
		if (syscall_read_dev((u_int)(dst + offset), 0x13004000, 0x200) != 0) {
			user_panic("read failed!\n");
		}
	
		offset += 0x200;
		
		// error occurred, then panic.
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
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int offset_now = 0;
	int op_status = 0;
	int write = 1;
	int can_read = 0;

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		// copy data from source array to disk buffer.
		offset_now = offset_begin + offset;
		if (syscall_write_dev(&diskno, 0x13000010, 4) != 0) {
			user_panic("write failed!\n");
		}
		if (syscall_write_dev(&offset_now, 0x13000000, 4) != 0) {
			user_panic("write failed!\n");
		}
		if (syscall_write_dev(src + offset, 0x13004000, 0x200) != 0) {
			user_panic("read failed!\n");
		}
		if (syscall_write_dev(&write, 0x13000020, 4) != 0) {
			user_panic("write failed!\n");
		}
		if (syscall_read_dev(&op_status, 0x13000030, 4) != 0) {
			user_panic("write failed!");
		}
		if (op_status == 0) {
			user_panic("read failed!\n");
		}
		
		offset += 0x200;
		// if error occur, then panic.
	 }
}

/* alter in lab5-1-exam */
int time_read()
{
	int time;
	if (syscall_write_dev(&time, 0x15000000, 4) != 0) {
		user_panic("write failed!\n");
	}
	if (syscall_read_dev(&time, 0x15000000 + 0x0010, 4) != 0) {
		user_panic("write failed!\n");
	}
	return time;
}






/* alter in lab5-1-exam finished*/
