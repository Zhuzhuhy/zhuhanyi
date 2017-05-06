#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
//	DATA_TYPE s=op_src->val;
	if(DATA_BYTE == 2){
    cpu.esp = cpu.esp - 2;
    swaddr_write(cpu.esp,2,op_src->val);
	}
	else {
	cpu.esp = cpu.esp -4;
//	uint32_t val=s;
	swaddr_write(cpu.esp,4,op_src->val);
	}
	print_asm_template2();
}

make_instr_helper(i)
//make_instr_helper(rm)
//#if DATA_BYTE==2||DATA_BYTE==4
make_instr_helper(r)
//#endif


#include "cpu/exec/template-end.h"
