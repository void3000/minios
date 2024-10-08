void switchtouseland(void);

void switchtouseland(void)
{
	asm volatile ("cli");
	__asm__ volatile(
		"mov $0x23, %ax;"  // Store user mode data selector in ax register
		"mov %ax, %ds;"    // Set ds, es, fs, and gs to user mode data selector
		"mov %ax, %es;"
		"mov %ax, %fs;"
		"mov %ax, %gs;"
		
		"mov %esp, %eax;"
		"pushl $0x23;"
		"pushl %eax;"
		"pushf;"
		
		// The next 3 lines are the same as `sti`
		"pop %eax;"        // Get EFLAGS back into EAX. The only way to read EFLAGS is to pushf then pop.
		"or $0x200, %eax;" // Set the IF flag.
		"push %eax;"       // Push the new EFLAGS value back onto the stack.
		"pushl $0x1B;"
		
		"push $1f;"
		"iret;"
		"1:"
	);
}
