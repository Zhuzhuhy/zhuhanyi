#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];
    uint8_t s[4096];
#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */
	//panic("please implement me");
#ifdef HAS_DEVICE
	ide_read(s,elf->e_phoff,elf->e_phentisize*elf->e_phnum);
#else
    ramdisk_read(s,elf->e_phoff,elf->e_phentsize*elf->e_phnum);
#endif
	ph=(void*)s;
	int j;
	for(j=0;j<elf->e_phnum;j++ ) {

		/* Scan the program header table, load each segment into memory */
		if(ph[j].p_type == PT_LOAD) {

			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */ 
		#ifdef HAS_DEVICE
		ide_read((void*)ph[j].p_vaddr,ph[j].p_offset,ph[j].p_filesz);
        #else
		ramdisk_read((void*)ph[j].p_vaddr,ph[j].p_offset,ph[j].p_filesz);
        #endif
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */  

         memset((void*)(ph[j].p_vaddr+ph[j].p_filesz),0,(ph[j].p_memsz-ph[j].p_filesz));
#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
