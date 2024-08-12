typedef struct
{
	unsigned int id;
	unsigned int stack;
	unsigned int state;
	unsigned char alias[32];
	void (*thread)();
}process_t;

process_t processes[2];
int pid = 0;

unsigned int task_switch(unsigned int esp);

void create_process(int id, unsigned char alias[32], void (*thread)());

void create_process(int id, unsigned char alias[32], void (*thread)())
{
	asm volatile ("cli");

	process_t* process	= (process_t*)malloc(sizeof(process_t));
	process->stack		= malloc(0x1000) + 0x1000;
	unsigned int *stack = (unsigned int*)process->stack;

	    // Expand down stack
        // processor data
        *--stack = 0x202;       // EFLAGS
        *--stack = 0x08;        // CS
        *--stack = (unsigned int) thread;       // EIP

        // pusha
        *--stack = 0;           // EDI
        *--stack = 0;           // ESI
        *--stack = 0;           // EBP
        *--stack = 0;           // NULL
        *--stack = 0;           // EBX
        *--stack = 0;           // EDX
        *--stack = 0;           // ECX
        *--stack = 0;           // EAX

        // data segments
        *--stack = 0x10;        // DS
        *--stack = 0x10;        // ES
        *--stack = 0x10;        // FS
        *--stack = 0x10;        // GS

		process->state	= 1;
		process->stack	= (unsigned int)stack;
		process->thread = thread;

		processes[id]	= *process;
		pid = id;

	asm volatile ("sti");
}

unsigned int task_switch(unsigned int esp)
{
	processes[pid].stack = esp;
	pid = (pid == 0) ? 1 : 0;
	return processes[pid].stack;
}

asm ("\
	 .global _task_interrupt;	\
	 .extern _task_switch;	\
	 \
	 _task_interrupt:	\
		pushal;	\
		push %ds;	\
		push %es;	\
		push %fs;	\
		push %gs;	\
		movw $0x10, %ax;	\
		movw %ax, %ds;	\
		movw %ax, %es;	\
		movw %ax, %fs;	\
		movw %ax, %gs;	\
		movl %esp, %eax;	\
		pushl %eax;	\
		call _task_switch;	\
		movl %eax, %esp;	\
		pop %gs;	\
		pop %fs;	\
		pop %es;	\
		pop %ds;	\
		popal;	\
		iret;	\
	 ");
