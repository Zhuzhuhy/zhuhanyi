#include "cpu/exec/template-start.h"

#define instr jle
static void do_execute(){

    if(cpu.EFLAGS.ZF == 1 && cpu.EFLAGS.SF!=cpu.EFLAGS.OF) 
	{	cpu.eip+=op_src->val;
	
		if(DATA_BYTE==2)
		
	
			cpu.eip&=0xffff;
		
}
	
      print_asm_template1();
}
make_instr_helper(i);


#include "cpu/exec/template-end.h"
