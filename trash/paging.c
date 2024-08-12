typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned bool;

#define PAGE_SIZE	(0x1000) /*4kb*/
#define ENTRIES		(0x400) /*1kb*/
#define ERROR	((void*)-1)
#define SUCCESS	((void*)0)
/*Kernel location*/
#define KERNEL_LOC	0xff400
#define KERNEL_SIZE 0x3000 /*12 Kb*/
/*page directory beginning address*/
#define PAGE_DIR_ADDR	(KERNEL_LOC + KERNEL_SIZE) /*right after the kernel*/
/*size of each page table in IA32 architecture*/
#define PAGE_TABLE_SIZE  0x400000
#define PAGE_DIR_INDEX(virt_addr) (virt_addr >> 22)
#define PAGE_TABLE_INDEX(virt_addr) (virt_addr >> 12)
/*next table. Where x > 0*/
#define NEXT_TABLE(x) (PAGE_TABLE_SIZE << (x - 1)) /*queues the next page table*/

/*page directory address pointer*/
uint32_t* page_dir = (uint32_t*)PAGE_DIR_ADDR;

void config_paging();
/*Invalidates a single page table entry*/
 void TLB_flush();
 /*Enables Paging size Extention*/
 void PSE_enable();
/* Keeps track of the last page (page table)
 * We initialize to zero because there's no
 * table created at start.
 */
uint32_t* last_page_table = 0;

/* Creating a page table.
 * It has 1024 entries that are each 4Kb in size
 * making up exaclty 4 MB per table.
 * Note, the physical address must be 4 Kb aligned.
 * Again, the G bit flag is not set, thus Translation Lookaside Buffer (TLB inside CPU) is not prevented from updating the address in it's
 * cache if Cr3 is reset.
 * D is also not set, thus page has not been written to.The CPU sets this bit automatically
 * and is up to the OS to set this bit to 0.
 * Set bit 0 and bit 1.
 * Bit 0, P (Present), if set, the page is actually in physical memory at the moment.
 * Bit 1, R (Read/Write) permission flag, if set, the page becomes readable/writable (see Intel Manual 3A 2-20).
 */
void paging_map_virtual_to_phys(uint32_t virt_addr, uint32_t phys)
{
    uint32_t id = PAGE_DIR_INDEX(virt_addr); /*move bit 22 to bit 0. Retrieve page directory index.*/
    uint32_t i = 0;
    /*takes up 4 MB*/
	for ( i; i < ENTRIES; i++ )
	{
		last_page_table[i] = phys | 3; /*Mark page: Supervisor, present and writeable/Readable*/
		phys += PAGE_SIZE; /*queue the next table*/
	}
	/* Set bit 0 and bit 1.
	 * Bit 0, P (Present), if set, the page is actually in physical memory at the moment.
	 * Bit 1, R (Read/Write) permission flag, if set, the page becomes readable/writable (see Intel Manual 3A 2-20).
	 */
	page_dir[id] =(uint32_t)(last_page_table) | 3; /*Mark page: Supervisor, present and writeable/Readable*/
	last_page_table = (uint32_t*)((uint32_t)last_page_table + PAGE_SIZE); /*queues the next page table*/
}
/* enable paging*/
inline void paging_enabe(void)
{
	uint32_t cr0 = 0;

	__asm__ __volatile__ ("movl %0, %%cr3"::"r"(page_dir));
	__asm__ __volatile__ ("movl %%cr0, %0":"=r"(cr0));
	cr0 |= 0x80000000; /*enable paging. setting bit 31 to 1*/
	__asm__ __volatile__ ("movl %0, %%cr0"::"r"(cr0));
}
/* Invalidating TBL entry. It takes a single linear address.
 * Everytime a frame is accessed, the frame "number" gets
 * cached by the TLB. This technology allows for faster paging
 * as the MMU does not have to check the corresponding frame address
 * in the page table. However, this could be a disadvantage because
 * if the frame is unmarked DIRTY, then the MMU wont know about the
 * update. As a consequence, we have to refresh the TLB.
 */
 void TLB_flush(uint32_t linear_addr)
 {
    __asm__ __volatile__ ("INVLPG (%0)"::"r"(linear_addr):"memory");
 }
 /* Paging size extention (PSE).
  * Is a feature of x86 processors that allows for pages larger than 4 Kb size.
  * The CPUID can be used to check CPU for compatibility.
  * This increases memory limit to 64 GB!
  */
void PSE_enable() /*ok we are not going to use it here for now.*/
{
    uint32_t eax = 0;
    __asm__ __volatile__ ("movl %%cr4, %0":"=r"(eax));
    eax |= 0x00000010;
    __asm__ __volatile__ ("movl %0, %%cr4"::"r"(eax) );
}

/*initialize paging*/
void config_paging()
{
	uint32_t i = 0;
	uint32_t* last_page_table = (uint32_t*)(PAGE_DIR_ADDR + ENTRIES); /*location of the (first) page following the page directory*/

    /* Set page directory. Mark page: Supervisor and writeable/Readable
     * Since each page table is 4 MB virtual address in size,
     * 1024 page directories allow us to have 4 GB of Virtual address space.
     */
	for ( i; i < ENTRIES; i++, page_dir[i] = 0 | 2 )

	/*map vitual address to physical memory*/
	paging_map_virtual_to_phys(0,0); /*table 1*/
	paging_enabe();
}
