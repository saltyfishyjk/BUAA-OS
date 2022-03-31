#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

LEAF(_my_getchar)
	li t0, 0xB0000000
	loop:
		lb t1, 0(t0)
		beq t1, zero, loop
		nop
	end_loop
	sb t1, 0(t0)
	
	li t2, 13
	bne t1, t2, end_check_r
	nop
	check_r:
		li t2, 10
		sb t2, 0(t0)
	end_check_r
	
	or v0, zero, t1
	jr jra
END(_my_getchar)

LEAF(_my_putchar)
	li t0, 0xB0000000
	sb a0, 0(t0)
	jr ra
END(_my_putchar)

LEAF(_my_exit)
	li t0, 0xB0000000
	sb zero, 0x10(t0)
	jr ra
END(_my_exit)	
