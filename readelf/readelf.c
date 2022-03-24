/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Shdr *shdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }

		// get section table addr, section header number and section header size
			ptr_sh_table = binary + ehdr -> e_shoff;
			sh_entry_count = ehdr -> e_shnum;
			sh_entry_size = ehdr -> e_shentsize;
			
			u_char *ptr_ph_table = NULL;
			ptr_ph_table = binary + ehdr -> e_phoff;
			Elf32_Half ph_entry_count = ehdr -> e_phnum;
			Elf32_Half ph_entry_size = ehdr -> e_phentsize;
			Elf32_Phdr *phdr = NULL;
			
			// flag = 0 : normal ; flag = 1 : Overlay ; flag = 2 : Conflict
			int flag = 0;
			int cnt = 0;
			int fileSizeArr[15];
			int memSizeArr[15]; 
			int addrSt = 0;
			int addrNow =0;
			int rLast = 0;
			for (Nr = 0;Nr < ph_entry_count;Nr++) {
				phdr = (Elf32_Phdr *)(ptr_ph_table + Nr * ph_entry_size);
				fileSizeArr[cnt] = phdr -> p_filesz;
				memSizeArr[cnt] = phdr -> p_memsz;
				cnt++;
				int l = phdr -> p_vaddr;
				int r = l + phdr -> p_memsz;
				if (Nr == 0) {
					rLast = r;
					addrSt = l + ((r - l) / 4096) * 4096;
					addrNow = addrSt;
				} else {
					if (l <= rLast) {
						printf("Conflict at page va : 0x%x\n", addrNow);
						flag = 2;
						return 0;
					}
					if (l < addrNow + 4096) {
						printf("Overlay at page va : 0x%x\n", addrNow);
						flag = 1;
						return 0;
					}
					rLast = r;
					addrNow = addrSt + ((r - addrSt) / 4096) * 4096;
				}
			}
			int i;
			if (flag == 0) {
				for (i = 0;i < cnt;i++) {
					printf("%d:0x%x,0x%x\n", i, fileSizeArr[i], memSizeArr[i]);
				}
			}
		// for each section header, output section number and section addr.
			//for (Nr = 0; Nr < sh_entry_count; Nr++) {
			//	shdr = (Elf32_Shdr *)(ptr_sh_table + Nr * sh_entry_size);
			//	printf("%d:0x%x\n", Nr, shdr->sh_addr);
			//}
		// lab1-1-Extra
			
        // get section table addr, section header number and section header size.

        // for each section header, output section number and section addr. 
        // hint: section number starts at 0.


        return 0;
}
