#include <asm/desc.h>

void my_store_idt(struct desc_ptr *idtr) {
// <STUDENT FILL> - HINT: USE INLINE ASSEMBLY

	asm("sidt %0;"
	:"+m"(*idtr)
	);
	return;

// </STUDENT FILL>
}

void my_load_idt(struct desc_ptr *idtr) {
// <STUDENT FILL> - HINT: USE INLINE ASSEMBLY

	asm("lidt %0"
	:	
	:"m"(*idtr)
	:
	);
	return;

// <STUDENT FILL>
}

void my_set_gate_offset(gate_desc *gate, unsigned long addr) {
// <STUDENT FILL> - HINT: NO NEED FOR INLINE ASSEMBLY

	unsigned long mask, long_one, low, mid, high;
	
	long_one = 1;
		
	mask = (long_one << 16) - 1;
	low = addr & mask;
	gate->offset_low = (unsigned short)low;

	mask = ((long_one << 16) - 1) << 16;
	mid = (addr & mask) >> 16;
	gate->offset_middle = (unsigned short)mid;

	mask = ((long_one << 32) - 1) << 32;
	high = (addr & mask) >> 32;
	gate->offset_high = (unsigned int)high;
	return;

// </STUDENT FILL>
}

unsigned long my_get_gate_offset(gate_desc *gate) {
// <STUDENT FILL> - HINT: NO NEED FOR INLINE ASSEMBLY
	unsigned long low, mid, high;	

	low = (unsigned long)(gate->offset_low);
	
	mid = (unsigned long)(gate->offset_middle);
	mid = (mid << 16);
	
	high = (unsigned long)(gate->offset_high);
	high = (high << 32);
	
	return low + mid + high;

// </STUDENT FILL>
}