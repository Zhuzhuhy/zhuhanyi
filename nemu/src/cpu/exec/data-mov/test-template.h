#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	op_dest->val = op_src->val & op_dest->val;
    cpu.EFLAGS.OF = 0;
	cpu.EFLAGS.CF = 0;
	DATA_TYPE num = op_dest->val;
	cpu.EFLAGS.SF = 0x01 & num ;
	if(num == 0) cpu.EFLAGS.ZF = 1;
	else  cpu.EFLAGS.ZF = 0;
	int x,y,i=0;

	x = 0x01 & num ;
	num = num >> 1;
	i++;
	while(i<=8){
	y = 0x01 & num ;
	num = num >> 1;
    x = !(x^y);
	i++;
	}
	cpu.EFLAGS.PF = x;
	OPERAND_W(op_dest, op_dest->val);
	print_asm_template2();
}

make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
