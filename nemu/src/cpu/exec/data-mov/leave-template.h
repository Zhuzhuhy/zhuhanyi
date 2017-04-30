#include "cpu/exec/template-start.h"

#define instr leave

/*static void do_execute() {
	if(DATA_BYTE == 2){
    cpu.esp = cpu.esp - 2;
    swaddr_write(cpu.esp,2,op_src->val);
	}
	else {
	cpu.esp = cpu.esp -4;
	swaddr_write(cpu.esp,4,op_src->val);
	}
	print_asm_template2();
}

make_instr_helper(i)
make_instr_helper(rm)
make_instr_helper(r)
*/
make_helper(concat(leave_,SUFFIX)){
	cpu.esp=cpu.ebp;
#if DATA_BYTE==2
	REG(R_BP)=MEM_R(REG(R_ESP));
#endif
#if DATA_BYTE==4
	REG(R_EBP)=MEM_R(REG(R_ESP));
#endif
    REG(R_ESP)+=DATA_BYTE;
	print_asm("leave");
	return 1;
}
#include "cpu/exec/template-end.h"
