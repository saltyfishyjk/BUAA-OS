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

/* alter in lab5-1-Extra */
int raid4_valid(u_int diskno)
{
	int ret;
	int op_status;
	//ide_read(diskno, 0, &ret, 1);
	int offset_now = 0;
	int read = 0;
	if (syscall_write_dev(&diskno, 0x13000010, 4) != 0) user_panic("sth went wrong!\n");
	if (syscall_write_dev(&offset_now, 0x13000000, 4) != 0) user_panic("sth went wrong!\n");
	if (syscall_write_dev(&read, 0x13000020, 4) != 0) user_panic("sth went wrong!\n");
	
	if (syscall_read_dev(&op_status, 0x13000030, 4) != 0) {
		user_panic("sth went wrong in raid4_valid!\n");
	}
	if (op_status == 0) {
		return 0;
	} else {
		return 1;
	}
}

void bxor512(void *a, void *b, void *c)
{
	int cnt = 512;
	while(cnt--) {
		*(char *)c = (*(char *)a) ^ (*(char *)b);
		a++;
		b++;
		c++;
	}
}



int raid4_write(u_int blockno, void *src)
{
	int i;
	int cnt = 0;
	int checknum[128];
	for (i = 0;i < 128;i++) {
		checknum[i] = 0;
	}
	void * va =  src;
	for (i = 1;i <= 5; i++) {
		if (i < 5)
			bxor512(va, checknum, checknum);
		if (!raid4_valid(i)) {
			cnt++;
			va += 0x200;
			continue;
		}
		if (i <= 4) {
			ide_write(i, 2 * blockno, va, 1);
		} else {
			ide_write(i, 2 * blockno, checknum, 1);
		}
		va += 0x200;
	}
	va -= 0x200;
	for (i = 0; i < 128 ;i++) {
		checknum[i] = 0;
	}
	for (i = 1;i <= 5; i++) {
		if (i < 5)
            bxor512(va, checknum, checknum);
        if (!raid4_valid(i)) {
            // cnt++;
			va += 0x200;
            continue;
        }
        if (i <= 4) {
            ide_write(i, 2 * blockno + 1, va, 1);
        } else {
            ide_write(i, 2 * blockno + 1, checknum, 1);
        }
		va += 0x200;
    }
	return cnt;
}

int checkeq512(void *a, void *b)
{
	int cnt = 512;
	while(cnt--) {
		char x = *(char *)a;
		char y = *(char *)b;
		if (x != y) {
			return -1;
		}
		a++;
		b++;
	}
	return 0;
}


int raid4_read(u_int blockno, void *dst)
{
	int cnt = 0;
	int i;
	int checknum[128];
	int content[128];
	void *va = dst;
	int flag = 0;
	for (i = 1; i <= 5; i++) {
		if (!raid4_valid(i)) {
			cnt++;
		}
	}
	if (cnt == 0) {
		for (i = 1; i <= 5;i++) {
			if (i <= 4) {
				ide_read(i, 2 * blockno, va, 1);
				if (i == 1) {
					user_bcopy(va, checknum, 512);
				} else {
					bxor512(va, checknum, checknum);
				}
			} else {
				ide_read(i, 2 * blockno, content, 1);
				if (checkeq512(checknum, content) != 0) {
					flag = -1;
				}
			}
			va += 0x200;
		}
		va -= 0x200;
		for (i = 1; i <= 5;i++) {
            if (i <= 4) {
                ide_read(i, 2 * blockno + 1, va, 1);
                if (i == 1) {
                    user_bcopy(va, checknum, 512);
                } else {
                    bxor512(va, checknum, checknum);
                }
            } else {
                ide_read(i, 2 * blockno + 1, content, 1);
                if (checkeq512(checknum, content) != 0) {
                    flag = -1;
                }
            }
            va += 0x200;
        }
		return flag;
	} else if (cnt == 1) {
		if (!raid4_valid(5)) {
			for (i = 1; i <= 4;i++) {
				ide_read(i, 2 * blockno, va, 1);
				va += 0x200;
			}
			for (i = 1; i <= 4;i++) {
                ide_read(i, 2 * blockno + 1, va, 1);
				va += 0x200;
			}
		} else {
			int flagn;
			for (i = 1; i <= 5; i++) {
				if (!raid4_valid(i)) {
					flagn = i;
					va += 0x200;
					continue;
				}
				if (i <= 4) {
					ide_read(i, 2 * blockno, va, 1);
				} else {
					ide_read(i, 2 * blockno, checknum, 1);
				}
				va += 0x200;
			}
			va -= 0x200;
			int ans[128];
			for (i = 0; i < 128; i++) ans[i] = 0;
			for (i = 1; i <= 5; i++) {
				if (i != flagn) {
					if (i <= 4) {
						bxor512(va - (0x200 * (4 - i)), ans, ans);
					} else {
						bxor512(checknum, ans, ans);
					}
				}
			}
			user_bcopy(ans, va - (0x200 * (4 - flag)), 512);
			for (i = 1; i <= 5; i++) {
                if (!raid4_valid(i)) {
                    flagn = i;
                    va += 0x200;
                    continue;
                }
                if (i <= 4) {
                    ide_read(i, 2 * blockno + 1, va, 1);
                } else {
                    ide_read(i, 2 * blockno + 1, checknum, 1);
                }
				va += 0x200;
            }
			va -= 0x200;
            // int ans[128];
            for (i = 0; i < 128; i++) ans[i] = 0;
            for (i = 1; i <= 5; i++) {
                if (i != flagn) {
					if (i <= 4) {
                   		bxor512(va - (0x200 * (4 - i)), ans, ans);
					} else {
						bxor512(checknum, ans, ans);
					}
                }
            }
            user_bcopy(ans, va - (0x200 * (4 - flag)), 512);
		}
		return 1;
	} else {
		return cnt;
	}
	
}

/* alter in lab5-1-Extra finished */
