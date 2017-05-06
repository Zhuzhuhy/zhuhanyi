#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	DATA_TYPE s=op_src->val;
	if(DATA_BYTE == 2){
    cpu.esp = cpu.esp - 2;
    MEM_W(cpu.esp,s);
	}
	else {
	cpu.esp = cpu.esp -4;
	uint32_t val=s;
	swaddr_write(cpu.esp,4,val);
	}
	print_asm_template2();
}

make_instr_helper(i)
make_instr_helper(rm)
make_instr_helper(r)



#include "cpu/exec/template-end.h"
