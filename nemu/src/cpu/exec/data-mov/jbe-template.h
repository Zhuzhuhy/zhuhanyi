#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute() {
    DATA_TYPE_S d=op_src->val;
    if(cpu.EFLAGS.ZF == 1|| cpu.EFLAGS.CF==1) {
		cpu.eip = cpu.eip + d;
	   if(DATA_BYTE==2)
		   cpu.eip&=0xffff;
	}
    print_asm_template1();
        
}
make_instr_helper(i);


#include "cpu/exec/template-end.h"
