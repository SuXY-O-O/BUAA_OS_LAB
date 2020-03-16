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

	int endingFlag = 0; // Chenk type: 0 small 1 big

	Elf32_Phdr *phdr = NULL;

	Elf32_Half ph_entry_count = ehdr->e_phnum;

	Elf32_Half ph_entry_size = ehdr->e_phentsize;

	Elf32_Off ph_offset = ehdr->e_phoff;

        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }
	else {
		// Check big or small
		if (*(binary + 5) ==  2) {
			endingFlag = 1;
			ph_entry_count = BSWAP_16(ph_entry_count);
			ph_entry_size = BSWAP_16(ph_entry_size);
			ph_offset = BSWAP_32(ph_offset);
		}
		else if (*(binary + 5) == 1) {
			endingFlag = 0;
		}
		else {
			printf("illegal formate\n");
			return 0;
		}
		phdr = (Elf32_Phdr *)((u_char *)ehdr + ph_offset);
		for (Nr = 0; Nr < ph_entry_count; Nr++) {
			if (endingFlag) {
				Elf32_Word fisz = BSWAP_32(phdr->p_filesz);
				Elf32_Word mesz = BSWAP_32(phdr->p_memsz);
				printf("%d:0x%x,0x%x\n", Nr, fisz, mesz);
			}
			else {
				printf("%d:0x%x,0x%x\n", Nr, phdr->p_filesz, phdr->p_memsz);
			}
			phdr = (Elf32_Phdr *)((u_char *)phdr + ph_entry_size);
		}
	}
        // get section table addr, section header number and section header size.

        // for each section header, output section number and section addr. 
        // hint: section number starts at 0.


        return 0;
}

